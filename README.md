## BoatShare - m2m eksamen 2020
 
### 1.0 Hardware
Alle klasser og metoder er dokumentert ved hjelp av doxygen og 
alt ligger vedlagt. Her er et raskt sammendrag av hva den fysiske delen av løsningen inneholder

#### 1.1 Particle photon
All håndtering av data som kommer via sim7600 modulen gjøres her.

#### 1.2 Sim 7600e lte/gps modul
Benyttes for å oppnå kontakt med omverden ved hjelp av 4g

#### 1.3 Songle relay
Relè for å kunne åpne og lukke låsen som er koblet til løsningen

#### 1.4 Fritzing

-------------------


### 2.0 Backend
Hele backend-løsningen er hostet ved hjelp av en digital ocean droplet. 
Her kjører Nginx som lar oss sende statiske filer til de som kobler seg opp på ip addressen/domenet(istre.io) til
dropleten. Disse statiske dokumentene er i dette tilfellet react appen sin build mappe.

#### 2.1 Hosting av løsningen på DigitalOcean 

##### 2.1.1 Droplet
Løsningen er hostet på en digital ocean droplet som kjører Ubuntu 20.04 (LTS) x64

##### 2.1.2 Nginx
Nginx er et server-verktøy som gjør det mulig styre trafikken som kommer inn på serveren basert på stien som er spesifisert i url'en.
Når en klient kobler seg opp sender nginx de statiske dokumentene tilbake. 

En viktig grunn til at jeg tok i bruk Nginx var mulighet til å gjøre såkalt "reverse-proxy" som vil si at backend løsningen(nodejs) kan lytte på localhost:9000 og vi slipper dermed å la apiet til backend 
ligge å lytte ut på den offentlige ip-addressen til serveren. Når en spørring fra frontend gjøres sendes dette til nginx serveren som lytter på den offentlige ip-addressen til serveren. 
Deretter sender nginx api-kallet videre inn på serveren, men nå blir api-kallets url endret til localhost og nodejs serveren blir dermed klar over at frontend har sendt en forespørsel.

##### 2.1.3 Lets encrypt og Certbot for https trafikk mellom klient og tjener
Serveren er satt opp med https-kryptering og dette er gjort ved hjelp av Certbot som enkelt oppretter og henter ssl sertifikater fra Lets encrypt(Virdó & Juell 2018) sin tjeneste.

#### 2.2 API ved hjelp av NodeJS
APIet i løsningen er opprettet ved hjelp av Nodejs. Årsaken til dette  valget er at jeg har litt erfaring med dette tidligere og visste at jeg raskt kunne sette opp en 
tilfredsstillende løsning. Følgende bibliotek har gjort det mulig å kommunisere med klient og particle photon i tillegg til å lagre data mellom hver sesjon.

##### 2.2.1 Expressjs

##### 2.2.2 Socket.io

##### 2.2.3 Mqtt

##### 2.2.4 Mongoose


------

#### Frontend - React
The
-----

#### Kilder
 - Virdó, Hazel  and  Juell, Kathleen 2018. "How To Secure Nginx with Let's Encrypt on Ubuntu 18.04" Hentet 10. nov 2020. https://www.digitalocean.com/community/tutorials/how-to-secure-nginx-with-let-s-encrypt-on-ubuntu-18-04

------