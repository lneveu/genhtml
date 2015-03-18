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
nodeStyle = genhtml.instanciate(nodeStyle,3,"nodeStyle",80,80);
genhtml.setboxstyle(boxNodeStyle,nodeStyle);


-- titleStyle
boxTitleStyle = genhtml.box(genhtml.root());
titleStyle = genhtml.resource("titleStyle");
titleStyle = genhtml.instanciate(titleStyle,1,"title");
genhtml.setboxstyle(boxTitleStyle,titleStyle);

-- title top-right
boxTitleStyle1 = genhtml.box(genhtml.root());
titleStyle1 = genhtml.resource("title_trStyle");
titleStyle1 = genhtml.instanciate(titleStyle1,1,"top-right");
genhtml.setboxstyle(boxTitleStyle1,titleStyle1);

-- title top-left
boxTitleStyle2 = genhtml.box(genhtml.root());
titleStyle2 = genhtml.resource("title_tlStyle");
titleStyle2 = genhtml.instanciate(titleStyle2,1,"top-left");
genhtml.setboxstyle(boxTitleStyle2,titleStyle2);

-- title bottom-right
boxTitleStyle3 = genhtml.box(genhtml.root());
titleStyle3 = genhtml.resource("title_brStyle");
titleStyle3 = genhtml.instanciate(titleStyle3,1,"bottom-right");
genhtml.setboxstyle(boxTitleStyle3,titleStyle3);

-- title bottom-left
boxTitleStyle4 = genhtml.box(genhtml.root());
titleStyle4 = genhtml.resource("title_blStyle");
titleStyle4 = genhtml.instanciate(titleStyle4,1,"bottom-left");
genhtml.setboxstyle(boxTitleStyle4,titleStyle4);


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
