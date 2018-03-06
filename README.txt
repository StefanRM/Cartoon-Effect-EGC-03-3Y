.___________. _______ .___  ___.      ___          ____   
|           ||   ____||   \/   |     /   \        |___ \  
`---|  |----`|  |__   |  \  /  |    /  ^  \         __) | 
    |  |     |   __|  |  |\/|  |   /  /_\  \       |__ <  
    |  |     |  |____ |  |  |  |  /  _____  \      ___) | 
    |__|     |_______||__|  |__| /__/     \__\    |____/  

Nume:    Maftei Stefan - Radu
Grupa:   336CC
Materie: EGC

	Pentru implementarea temei am folosit functiile si fisierele din
laboratoarele 8 (modelul Phong si luminile) si 9 (texturarea). Rezultatul se
afla in laboratorul 9.
	In scena se afla modelul de Mos Craciun, cu texturile oferite, un cer gri
si pamant, pe care se reflecta luminile. Catre shadere se trimit informatii
utile ca pozitia luminilor, constantele de material, culorile obiectelor,
caracteristicile lumii (pozitia si normala), semafoare pentru identificarea
mosului, luminilor si conturului.
	Luminile sunt in numar de 4, fiind imprastiate in jurul mosului. Acestea
au o miscare dinamica de forma unei inimi (cea rosie), Lisajous (albastru si
verde) si spirala (turcoaz).
	Efectul de toon outline(stroke) are loc in vertex shader sub forma de
deplasare a vertecsilor, iar in fragment shader de colorare cu o culoare
constanta, conform enuntului. Se aplica in scena glCullFace(GL_FRONT) pentru
a face vizibile obiectele din spate.
	Efectul de stepped lighting se afla in fragment shader. Aici se aplica
formula din enunt pe intensitatea difuza si speculara a fiecarei lumini,
conform enuntului.
	Iluminarea se aplica pentru fiecare lumina din scena, apoi se aduna toate
rezultatele pentru a rezulta culoarea finala. In cazul mosului, proprietatile
kd (difuza) si ks (speculara) se obtin din texturile oferite.
	Prin apasarea unei taste se trece de la modelul de iluminare Phong la toon
shading, ambele fiind prezente in cadrul aplicatiei.	

Taste control:
-> T pentru a activa toon shading;
-> P pentru a porni / opri miscarea luminilor;
-> U / I pentru cresterea / scaderea lui numLevels.