## BoatShare - m2m eksamen 2020

### Introduksjon
Oppgaven var å lage en løsinge som kunne 

### 1.0 Particle Photon

Alle klasser og metoder er dokumentert ved hjelp av doxygen og 
alt ligger vedlagt. Her er et raskt sammendrag av hva den fysiske delen av løsningen inneholder

#### 1.1 Sim 7600e lte/gps modul
Benyttes for å oppnå kontakt med omverden ved hjelp av 4g i tillegg til å muliggjøre sporing av båten

#### 1.2 Songle relay
Relé for å kunne åpne og lukke låsen som er koblet til løsningen


-------------------


### 2.0 Backend
Hele backend-løsningen er hostet ved hjelp av en digital ocean droplet. 
Her kjører Nginx som lar oss sende statiske filer til de som kobler seg opp på ip addressen/domenet(istre.io) til
dropleten. Disse statiske dokumentene er i dette tilfellet react appen sin build mappe.

#### 2.1 Hosting av løsningen på DigitalOcean 
Valget om å benytte meg av DigitalOcean ble tatt da jeg så hvor raskt og enkelt man kunne få en ubuntu server opp og gå.

##### 2.1.1 Droplet 
Siden digital ocean enkelt og billig lar meg opprette en server har jeg valgt å opprette en droplet som kjører Ubuntu 20.04 (LTS) x64 hos dem

##### 2.1.2 Nginx
Nginx er et server-verktøy som gjør det mulig styre trafikken som kommer inn på serveren basert på stien som er spesifisert i url'en.
Når en klient kobler seg opp sender nginx de statiske dokumentene tilbake. 

En viktig grunn til at jeg tok i bruk Nginx var mulighet til å gjøre såkalt "reverse-proxy" som vil si at backend løsningen(nodejs) kan lytte på localhost:9000 og vi slipper dermed å la apiet til backend 
ligge å lytte ut på den offentlige ip-addressen til serveren. Når en spørring fra frontend gjøres sendes dette til nginx serveren som lytter på den offentlige ip-addressen til serveren. 
Deretter sender nginx api-kallet videre inn på serveren, men nå blir api-kallets url endret til localhost og nodejs serveren blir dermed klar over at frontend har sendt en forespørsel.(Tagliaferri & Juell 2020)

##### 2.1.3 Lets encrypt og Certbot for https trafikk mellom klient og tjener
Serveren er satt opp med https-kryptering og dette er gjort ved hjelp av Certbot som enkelt oppretter og henter ssl sertifikater fra Lets encrypt sin tjeneste.(Brian 2020) 

#### 2.2 CloudMQTT.com
Løsningen benytter seg av CloudMQTT som mqtt-broker.
For benytte denne trenger mqtt klientene bruker navn og passord.    65534
Se "Oppsett og viktig informasjon" nederst i dokumentet

#### 2.3 API ved hjelp av NodeJS
APIet i løsningen er opprettet ved hjelp av Nodejs. Årsaken til dette  valget er at jeg har litt erfaring med dette fra tidligere og visste at jeg raskt kunne sette opp en 
tilfredsstillende løsning for et API. Følgende bibliotek har gjort det mulig å kommunisere med klient og particle photon i tillegg til å lagre data mellom hver sesjon.

##### 2.3.1 Expressjs 
Nodejs bibliotek for å opprette en webserver og håndtere forespørsler fra klient.
Her håndteres alle GET og POST request som går mellom klient og tjener. I dette tilfellet vil dette være 
informasjon om posisjonene til de ulike båtene og om en klient har forespurt åpning av en av disse. (Express)

##### 2.3.2 Socket.io
Jeg forsøkte først å benytte meg av såkalte SSE(Server Sent Events), men det viste seg at ubuntu sin brannmur stoppe dette ganske raskt.
For å kunne gi brukerne live data når de besøker tjenesten måtte jeg derfor sette opp websockets som kunne strømme data live fra serveren.
Det endte med at jeg tok i bruk Socket.io som viste seg å være veldig raskt og enkelt å få opp. 
Hver gang en klient kobler seg opp mot tjeneren opprettes de nødvendige websocketene slik at de får live posisjonsdata for alle båtene som er koblet opp mot løsningen.
(Socket.io)

##### 2.3.3 mqtt.js
Kommunikasjon med cloudMqtt gjøres med mqtt.js biblioteket. 
(mqtt.js)

##### 2.3.4 Mongoose
For å kunne lagre data om båtene mellom hver oppdatering og ved eventuell nedetid på serveren trengte jeg noe som kunne lagre data mellom hver økt.
Det endte med Mongoose da dette biblioteket enkelt lar nodejs serveren koble seg opp mot en MongoDB database. 
Siden MongoDB baserer seg på json gjør det databehandling med mqtt-meldingene veldig rask og enkel. 

------

#### 3.0 Frontend - React
For å kunne vise data til sluttbruker har jeg bnyttet React for å lage en enkel frontend som gir brukeren mulighet 
til å se hvor de ulike båtene befinner seg i tillegg til å kunne reservere/låse opp en båt. 
Løsningen er responsiv som dermed gjør det enkelt for brukere på både mobil og desktop.

##### 3.1 Bibliotek
###### 3.1.1 Google maps
Landingssiden for løsningen viser et kart over hvor de ulike båtene befinner seg. Dette kartet 
er laget ved hjelp av google-map-react for å tegne kart og markørene på kartet.
Markørene er egne komponenter som inneholder info om koordinater, id og om båten er pålogget.
Hvis båten ikke er pålogget forsvinner båten fra kartet og kan dermed ikke leies lenger.
(google-map-react)

###### 3.1.2 Socket.io
Samme som i backend, bare her benyttes en lytter for å lytte etter dataene som sendes via websockets til frontend.
(Socket.io frontend)

##### 3.2 React Komponenter

###### 3.2.1 Map
Hovedkomponenten i løsningen. Denne laster inn kartet og alle markørene som viser hvor de ulike båtene som er koblet opp mot løsningen befinner seg

###### 3.2.2 Marker
Komponent for markørene som vises i Map komponenten.
Benytter seg av posisjonsdata sendt fra serveren til å tegne seg selv opp inne i kart komponenten.

###### 3.2.2 Infowindow
Komponent for vinduet som dukker opp når brukeren klikker på en av markørene. 
Inneholder data om id, posisjon og en knap som lar brukeren reservere en båt.

###### 3.2.2 Burger
Komponent for burgermeny ikonet oppe i venstre hjørnet av løsningen.

###### 3.2.3 Menu
Komponent for menyen som kommer til syne når brukeren klikker på burgerikonet. 

-------

#### 4.0 Videreutvikling og hva som kunne vært gjort annerledes

##### 4.1 Valg av hardware
Løsningen bnytter ikke noe hardware utover det som er nødvendig for kunne gjennomføre den oppgaven som tingen skal gjøre, å spore og åpne/låse en båt. 
Jeg gikk tidlig inn for å benytte meg av particle photon da denne gir meg muligheten til å oppdatere firmware over internet. 

#### 5.0 Oppsett og viktig informasjon
Webserveren med nginx er ikke nødvendig for å teste løsningen

##### 5.1 Forutsetninger
- Prosjektet kjører med sikkerhet på Windows 10 og Ubuntu 20.04 (LTS) x64

##### 5.2 Installasjon av løsningen

1. Installere siste versjon av nodejs og npm https://nodejs.org/en/download/
	- Etter installasjon se til at både nodejs og npm er installert ved å kjøre følgene i terminalen:
```	
node -v
npm -v
```
2. Pakk ut zipfilen på ønsket sted
3. Gå til boat_share/ og kjør:
```npm install```
3. Gå til boat_share/boat_share_backend og kjør:
```npm install```
4. Bli i backend mappen og kjør:
```npm start``` 
5. Gå til boat_share/ og kjør:
```npm start``` 
6. Løsningen kjører nå på localhost:3000

##### Installasjon
###### NPM

##### Fritzing



-----

#### Kilder
 - Boucheron, Brian  2020. "How To Secure Nginx with Let's Encrypt on Ubuntu 20.04" Hentet 10. nov 2020. https://www.digitalocean.com/community/tutorials/how-to-secure-nginx-with-let-s-encrypt-on-ubuntu-20-04
 - Lisa Tagliaferri &  Kathleen Juell  2020. "How To Set Up a Node.js Application for Production on Ubuntu 20.04" Hentet 30. okt 2020 https://www.digitalocean.com/community/tutorials/how-to-set-up-a-node-js-application-for-production-on-ubuntu-20-04
 - Express 2020. Hentet 25. nov 2020. https://expressjs.com/en/4x/api.html
 - mqtt.js 2020. Hentet 25. nov 2020. https://www.npmjs.com/package/mqtt#api
 - Socket.io 2020. Hentet 25. nov 2020. https://socket.io/docs/v3/server-api/
 - Socket.io frontend 2020. Hentent 25 nov 2020. https://socket.io/docs/v3/client-api/index.html
 - google-map-react 2020. Hentet 25. nov 2020. https://github.com/google-map-react/google-map-react
------
