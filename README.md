C++ WebServer
===================

Prosjektoppgaven i C++ 2013

## Abstrakt

Målet er å lage en liten effektiv webserver kryss-kompatibel med Mac, Windows og Linux. Web Serveren skal **ikke** støtte annet enn GET. HTML/JS/CSS filer skal sendes "as is" som plain text, mens dynamiske nettsider skal lastes via linket C++-program. 

## Mål

* Abstrahert - webserveren skal lett kunne utvides i senere tid, med støtte for eksempelvis POST
* WebServeren bør kunne integreres mot andre språk, som for eksempel PHP, i senere tid.
* Siden WebServeren er liten, må også effektiviteten være på topp. Det vil si rask responstid, lav CPU og minnebruk.
* WebServeren bør kunne skaleres ved mange requests

## Gruppeinformasjon
David Steinsland, s180486, david@davidsteinsland.net