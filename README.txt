GEORGIAN ANDREEA - 331 CB
-------------------------
TEMA 1 - ASTROKITTY
-------------------------
Pentru a implementa tema conform cerintelor am procedat astfel:
- Crearea geometriei si desenarea asteroizilor dinamici:
	* astronaut - astronautul este un triughi echilateral
	* platforme - platformele sunt dreptunghiuri de diferite culori: platforma 
				  verde este platforma de reflexie, cea galbena este platforma 
				  de stationare, iar platforma rosie este de finish;
	* asteroizi - am desenat 3 asteroizi dinamici, fiecare dintre ei executand 
				  miscari de translatie(galbui), rotatie(albastru) si scalare
				  (roz);
				  
- Deplasarea astronautului la click in directia aleasa si orientare acestuia 
in directia de deplasare
	Pentru implementarea acestei cerinte am folosit metoda 
void AstroKitty::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
in cadrul careia am verificat daca butonul mouse-ului a fost apasat. In cazul 
in care acest lucru s-a intamplat, blochez initial utilizatorul din a mai 
controla astronautul prin modificarea unei variabile pentru miscare in 1. 
Daca se intampla ca astronautul sa ajunga pe platforma de statioanre, atunci 
vom modifica valoarea acestei variabile in 0 pentru a putea sa relansam 
astronautul. De asemenea, la lansarea astronautului vom memora pozitia in care 
s-a inregistrat click-ul si vom initializa coordonatele pozitiei de start 
cu pozitia initiala a triunghiului, coordonate ce se vor modifica pe parcursul 
deplasarii obiectului. Unghiul sub care se va roti astronautul pentru a se 
orienta in directia de deplasare l-am calculat folosind formula unghiului 
dintre doi vectori:
	unghi = atan2(capat2.y - capat1.y, capat2.x - capat1.x)
	
- Reflexie la coliziunea cu platformele de reflexie
	Pentru a detecta coliziunea dintre oricare doua drepte (P1,Q1) si (P2,Q2) 
am considerat ca ele se intersecteaza numai in cazul in care:
	* (P1, Q1, P2) si (P1, Q1, Q2) au orientari diferite
	* (P2, Q2, P1) si (P2, Q2, Q1) au orientari diferite
	* (P1, Q1, Q2), (P1, Q1, Q2), (P2, Q2, P1) si (P2, Q2, Q1) sunt coliniare
	* proiectia x a (P1,Q1) si (P2, Q2) intersecteaza proiectia y 
	a (P1,Q1) si (P2,Q2)
	Daca detectez o coliziune intre o dreapta a astronautului si o dreapta a 
platformei de reflexie, care se afla pe orizontala, proiectez pozitia actuala 
a astronautului fata de axa Oy si recalculez unghiul de deplasare al obiectului.
	In cazul coliziunii cu una din marginile scenei, din cele aflate la 
verticala, procedura este asemanatoare, insa proiectia pozitiei actuale se face 
fata de axa Ox.

- Detectarea coliziunii cu platformele de stationare (incetarea miscarii) si 
repozitionarea astronautului odata ajuns pe platforma de stationare
	Asa cum am mentionat si mai sus, in momentul in care se inregistreaza 
apasarea butonului mouse-ului, o variabila isi schimba valoarea din 0 in 1, iar 
aceasta se reseteaza numai in momentul in care astronautul atinge o platforma 
de stationare. De asemenea, miscarea obiectului este conditionata de valoarea 
acestei variabile, care trebuie sa fie 1 pentru ca acesta sa isi modifice 
pozitia.
	Asadar, in momentul in care este detectata coliziunea cu o platforma de 
stationare, se identifica punctul de coliziune (folosind formula punctului de 
intersectie a doua drepte), iar astronautul este translatat in acest punct. 
Dupa plasarea lui pe punctul resprectiv, ii este aplicata o rotire astfel incat 
orientarea triunghilui sa fie cu varful in jos in momentul in care intersecetaza 
latura inferioara a platformei, si cu varful in sus atunci cand punctul de 
intersectie se afla pe latura superioara.

- Distrugerea asteroidului la coliziune si modificarea directiei dupa coliziunea 
cu un asteroid
	Pentru a detecta coliziunea dintre astronaut si un asteroid, am aproximat 
obiectul din urma cu un cerc, care are raza si niste coordonate ale centrului. 
Se considera ca triunghiul si asteroidul se intersecteaza in momentul in care 
distanta dintre varful triunghiului( varful dupa care se face orientarea in 
directia de mers) si centrul cercului circumscris asteroidului este mai mica 
decat raza celui din urma. In momentul in care functia care verifica acest lucru 
intoarce true, atunci asteroidul este sters, iar directia de deplasare a 
astronautului este schimbata.

- Oprirea jocului la atingerea pozitiei finale
	Intersectia astronautului cu platforma de finish este verificat la fel ca si 
in cazul patformelor de reflexie si stationare. In momentul in care o intersectie 
este identificata cu succes, jocul se incheie, iar fereastra este inchisa.

