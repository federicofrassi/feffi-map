Come spiegato nel README, l’obiettivo di feffi-map è creare un piccolo scanner di rete ispirato a Nmap, implementando solo alcune funzionalità fondamentali.
La prima feature che ho scelto di replicare è una versione semplificata di nmap -sn su rete locale, basata su ARP host discovery.

Il primo passo è stato creare il main, che inizialmente si occupa solo di verificare che il comando sia scritto correttamente: numero corretto di argomenti, opzione -sn, opzione -i, nome dell’interfaccia e target.

Successivamente ho implementato un modulo dedicato alle informazioni dell’interfaccia di rete. Prima di poter inviare una richiesta ARP, infatti, feffi-map deve sapere da quale interfaccia sta lavorando e quali sono le informazioni locali dell’host: nome dell’interfaccia, interface index, MAC address e indirizzo IPv4.

Questa logica è dichiarata in iface.h e implementata in iface.c. Il nome dell’interfaccia viene passato come parametro e copiato nella struct iface_info_t. L’interface index viene ottenuto con if_nametoindex(), disponibile su Linux tramite <net/if.h>. Questo indice è importante perché le socket raw a livello Ethernet usano l’indice numerico dell’interfaccia, non solo il nome testuale come eth0.

Per ottenere il MAC address ho creato una socket temporanea e ho usato ioctl() con una struct ifreq. In particolare, copiando il nome dell’interfaccia dentro ifr.ifr_name e chiamando ioctl() con SIOCGIFHWADDR, il kernel riempie la struttura con l’hardware address dell’interfaccia. Da lì copio i 6 byte del MAC address dentro la mia struct.

Per ottenere l’indirizzo IPv4 ho usato una logica simile, sempre con ioctl() e struct ifreq, ma usando SIOCGIFADDR. Il kernel restituisce l’indirizzo dell’interfaccia dentro ifr.ifr_addr, che viene interpretato come struct sockaddr_in. Da lì estraggo sin_addr.s_addr e lo salvo nella struct come indirizzo IP locale.

A questo punto feffi-map è in grado di riconoscere le proprie informazioni locali. Il passo successivo è stato iniziare l’host discovery tramite ARP. Prima di gestire un’intera subnet in notazione CIDR, per esempio 192.168.110.0/24, ho deciso di supportare un singolo target IP.

Nel modulo arp.c, dichiarato tramite arp.h, ho implementato una funzione che costruisce una ARP request. Una richiesta ARP su Ethernet è composta da un Ethernet header di 14 byte e da un payload ARP di 28 byte, per un totale di 42 byte.

Per costruire il pacchetto ho usato un buffer di 42 byte. I primi byte vengono interpretati come struct ether_header, fornita dagli header di sistema, mentre la parte successiva viene interpretata come una struct ARP definita da me. L’Ethernet header contiene il MAC di destinazione, il MAC sorgente e l’EtherType. Nel caso di una ARP request, il MAC di destinazione è il broadcast ff:ff:ff:ff:ff:ff, il MAC sorgente è il MAC locale dell’interfaccia, e l’EtherType indica ARP.

Il payload ARP contiene invece informazioni come hardware type, protocol type, opcode, sender MAC, sender IP, target MAC e target IP. Per una ARP request, l’opcode indica “request”, il sender MAC/IP sono quelli locali, il target MAC è sconosciuto e quindi impostato a zero, mentre il target IP è l’indirizzo che vogliamo risolvere.

Dopo aver costruito il frame, ho preparato una struct sockaddr_ll, che serve a dire al kernel su quale interfaccia inviare il frame Ethernet. Infine ho usato sendto() sulla raw socket AF_PACKET per inviare effettivamente la richiesta in broadcast. Ho verificato il risultato con Wireshark filtrando per arp, osservando una richiesta del tipo Who has 192.168.110.1? Tell 192.168.110.21.

Il progetto è attualmente work in progress: la prossima fase sarà ricevere la ARP reply, controllare che provenga dal target corretto ed estrarre il MAC address dell’host remoto.
