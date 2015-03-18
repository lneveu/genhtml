Librairies nécessaires :
- libxml2
- igraph (http://igraph.org/c/#startc)
(+ set LD_LIBRARY_PATH variable -> "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:{path_to_lib_igraph}" )


constellationGen.lua : script lua de génération HTML du graph_AUTEUR
libForLua.c : librairie C contenant les fonctions utilisées par le scrit de génération
strabicGenHTML.sh : script bash pour générer une page HTML à partir d'un graph et d'un fichier de sortie
infile.xml  : fichier de ressource/contenu
resfile.xml : fichier de pattern des box

