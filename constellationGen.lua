require("genhtml");

-- parameters for bash
if ((arg[1] == nil) and (arg[2] == nil)) then
 	name_graph = "graph_SAISON.graphml";
	name_outfile = "constellation.html";
else
	name_graph = arg[1];
	name_outfile = arg[2];
end

genhtml.start(name_outfile);
genhtml.setpagewidth(1000);
genhtml.setpageheight(1000);

-- nodeStyle 1
boxNodeStyle = genhtml.box(genhtml.root());
nodeStyle = genhtml.resource("nodeStyle");
nodeStyle = genhtml.instanciate(nodeStyle,3,"nodeStyle",60,60);
genhtml.setboxstyle(boxNodeStyle,nodeStyle);

-- titleStyle
boxTitleStyle = genhtml.box(genhtml.root());
titleStyle = genhtml.resource("titleStyle");
titleStyle = genhtml.instanciate(titleStyle,1,"title");
genhtml.setboxstyle(boxTitleStyle,titleStyle);

-- authorStyle
boxAuthorStyle = genhtml.box(genhtml.root());
authorStyle = genhtml.resource("authorStyle");
authorStyle = genhtml.instanciate(authorStyle,1,"author");
genhtml.setboxstyle(boxAuthorStyle,authorStyle);

genhtml.generateConstellation(name_graph);
--genhtml.generateConstellation("graphe.xml");


-- Generate the HTML
genhtml.finish();
return
