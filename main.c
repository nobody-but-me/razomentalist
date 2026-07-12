
#include <stdio.h>
#define CAROL_IMPLEMENTATION
#include "carol.h"

#define CENTER_COLUMN "display: flex; justify-content: center; flex-direction: column; align-items: center;"
#define CENTER_ROW    "display: flex; justify-content: center; flex-direction: row; align-items: center;"

#define CENTER_SPACE_AROUND "display: flex; justify-content: space-around; align-items: center;"
#define CENTER_SPACE_BETWEEN "display: flex; justify-content: space-between; align-items: center;"

#define FLEX_BEGIN "display: flex; justify-content: begin; align-items: begin;"
#define FLEX_END "display: flex; justify-content: end; align-items: end;"

#define SOLID_BORDER(color) "border: 2px solid " #color ";"

static void return_index(void)
{
	div_begin(CENTER_SPACE_BETWEEN"margin-top: 15px; margin-bottom: 15px");
		para("[", NULL); hyperlink("./index.html", "Go Back To The Old House", NULL); para("]", NULL);
	div_end();
	return;
}

static void chess_background(void)
{
	div_begin("height: 100vh; width: 100%; background-repeat: repeat; background-size: 100px 100px;"
			  "background-image: url(https://raw.githubusercontent.com/nobody-but-me/razomentalist/main/media/grid.png);"
			  "animation: texture_loop 10s linear infinite;"
			);
	div_end();
	return;
}

void carol_compose(void)
{
	add_media("./index.css", "../index.css"); // dirty trick because carol is incomplete
	
	add_media("./media/neighbours.png", "neighbours.png");
	add_media("./media/journal.png", "journal.png");
	add_media("./media/writing.png", "writing.png");
	add_media("./media/about.png", "about.png");
	add_media("./media/math.png", "math.png");
	add_media("./media/log.png", "log.png");
	
	add_media("./media/grid.png", "grid.png");
	
	page_conf index_conf, about_conf;
	page index, about;
	
	create_configuration("As Gooffee As Me", get_var_name(index),"index",&index_conf);
	create_configuration("About", get_var_name(about), "index", &about_conf);
	
	// ABOUT PAGE
	page_begin(&about, &about_conf);
		div_begin(FLEX_BEGIN"width: 100vw; height: 100vh;");
		div_begin("width: 55%; height: 100%; overflow-y: scroll; overflow-x: hidden; padding-right: 25px; padding-left: 15px;");
			div_begin(CENTER_SPACE_BETWEEN"width: 100%;");
				header("About", ""); header("·", NULL); header("About", "transform: scaleX(-1);");
			div_end();
			div_begin(CENTER_COLUMN"width: 100%;");
				div_begin(CENTER_ROW"width: 100%");
					para("\"Olho minha caixa postal. Nada de novo. Volto ao 'principal'. Também nada de novo. Procuro aqui e acolá "
					"por algo, mas 'algo' algum existe. Talvez falte um quê de fantástico nas conversas do dia-a-dia, digo a mim mesmo "
					"numa mudez impressionante.",
				"width: 100%; text-align: justify;");
					empty_space("width: 30%");
				div_end();
				div_begin(CENTER_ROW"width: 100%;");
					empty_space("width: 30%");
					para("\"Levanto e vou ao banheiro. Lavo as mãos; seco-as. Me olho no espelho e sinto gemido entalado na garganta"
					". Sempre que penso não ser, acabo, prisioneiro, sendo. Eu, inextricavelmente eu -- concordo em "
					"sintônia comigo mesmo no espelho. Volto ao quarto. Nada de novo, repito, esquizofrênico.",
				"width: 100%; text-align: justify;");
				div_end();
				div_begin(CENTER_ROW"width: 100%");
					para("\"Nas mansas perguntas diárias, o que falta é o quê filosófico! Sim, sim! Mas... Mas o que exatamente seria esse quê? Substantivo? "
					"Próprio ou indefinido? Não sei -- não saberia.",
				"width: 100%; text-align: justify;");
					empty_space("width: 30%");
				div_end();
				div_begin(CENTER_ROW"width: 100%;");
					empty_space("width: 30%");
					para("\"Certa vez ouvi um cientista dizer que sente-se bem com a própria ignorância sobre certas questões sobre certas "
					"coisas; achei um absurdo: como pode um cientista se satisfazer tão completamente com algo tão pouco, tão insosso, sem sal,"
					" tempero ou caldo como a ignorância? Sempre desejo saber o porquê, olhando por baixo, sempre por baixo. Essa é a origem do"
					" ego cientista? Profundamente egocêntrico, guardo fundo ao peito tímido todos os sonhos do mundo -- mundo esse "
					"que conquisto todos os dias antes de levantar da cama.",
				"width: 100%; text-align: justify;");
				div_end();
				div_begin(CENTER_ROW"width: 100%;");
					para("\"Preso a superlativos e redundâncias, sempre fui razomentalista.",
				"width: 100%; text-align: justify;");
					empty_space("width: 30%");
				div_end();
			div_end();
			div_begin("min-width: 80%; padding-left: 25px;");
				para("I am a computation and physics undergraduate from Brazil.", NULL);
				div_begin(FLEX_BEGIN);
					header("About this website", NULL); header("&nbsp;&nbsp;·&nbsp;&nbsp;", NULL); header("About this website", "transform: scaleX(-1);");
				div_end();
				para("This website is built with <a href='https://github.com/nobody-but-me/carol.git' target='blank'>Carol</a>."
					 " Carol is a static website generator created by me; it's written in C23 and is highly inspired by "
					 "<a href='https://j3s.sh/thought/my-website-is-one-binary.html' target='blank'>j3s</a> and "
					 "<a href='https://iwillneverbehappy.neocities.org' target='blank'>Imya</a>'s pigeonhole. Its usefulness is "
					 "questionable and I wouldn't recommend it to anyone besides myself.",
				"width: 100%; text-align: justify;");
				para("Furthermore, Carol and this website -- as well as most of my programming projects -- are written with "
					 "<a href='https://github.com/nobody-but-me/charlie.git' target='blank'>Charlie</a>, my own minimal text editor "
					 "written in C23; it's basically a fork of <a href='https://github.com/antirez/kilo' target='blank'>Kilo</a>"
					 " with <a href='https://www.gnu.org/savannah-checkouts/gnu/emacs/emacs.html' target='blank'>Emacs</a> "
					 "keybinds.",
				"width: 100%; text-align: justify;");
				para("All the drawings and texts in this website are created by me and are under the "
					 "<a href='https://creativecommons.org/licenses/by-nc-sa/4.0/' target='blank'>CC BY-NC-SA 4.0</a> license.",
				"width: 100%; text-align: justify;");
				div_begin(FLEX_BEGIN);
					header("Interests", NULL); header("&nbsp;&nbsp;·&nbsp;&nbsp;", NULL); header("Interests", "transform: scaleX(-1);");
				div_end();
				para("Or what you are likely going to find in this website:", NULL);
				unordered_list_begin(NULL);
					list_item("Natural sciences (mainly physics and biology);", NULL);
					list_item("Computation and recreational programming;", NULL);
					list_item("Mathematics;", NULL);
					list_item("Literature, writing and linguistics;", NULL);
					list_item("Drawing and painting;", NULL);
					list_item("Metaphysics, Epistemology, Logic, Ethics and Asthetics discussions and thinking "
							  "-- Philosophy, in short."
					, NULL);
				unordered_list_end();
				div_begin(FLEX_BEGIN);
					header("More", NULL); header("&nbsp;&nbsp;·&nbsp;&nbsp;", NULL); header("More", "transform: scaleX(-1);");
				div_end();
				sub_header("Random facts about me listed by the Authoritarian Venus:", NULL);
				unordered_list_begin(NULL);
					list_item("daniel wears ladybug slippers;", NULL);
					list_item("daniel loves crocodiles;", NULL);
					list_item("daniel has a passion for philosophy;", NULL);
					list_item("daniel was learning to play the keyboard;", NULL);
					list_item("daniel is learning Russian.", NULL);
				unordered_list_end();
				sub_header("Contact:", NULL);
				unordered_list_begin(NULL);
					list_item("E-mail: asgooffeeasme@gmail.com;", NULL);
					list_item("<a href='https://signal.org/' target='blank'>Signal</a>: @dannyboy.72.", NULL);
				unordered_list_end();
			div_end();
			return_index();
		div_end();
		chess_background();
	page_end();
	
	// INDEX PAGE
	page_begin(&index, &index_conf);
		div_begin(CENTER_ROW"scale: 80%; min-width: 100%; width: 100%; min-height: 100%; height: 100%; margin: 0px; padding: 0px;");
			div_begin(CENTER_COLUMN"min-height: 100%;");
				empty_space("min-height: 70px;");
				div_begin("min-width: 100%; display: flex; justify-content: end; flex-direction: row; align-items: end;");
					image("journal.png", "journal room", "width: 225px;");
				div_end();
				image("neighbours.png", "neighbours room", "width: 250px;");
			div_end();
			div_begin(CENTER_COLUMN);
				div_begin(CENTER_ROW);
					image("math.png", "mathematics room", "width: 490px;");
				div_end();
				div_begin(CENTER_ROW);
					link_image("./about.html", "about.png", "about room", "width: 200px;");
					image("writing.png", "writing room", "width: 300px;");
				div_end();
				div_begin(CENTER_ROW);
					image("log.png", "logging room", "width: 505px;");
				div_end();
			div_end();
		div_end();
	page_end();
	return;
}

int main(int argc,char**argv)
{
	if ((carol_init())==-1)
		return -1;
	return 0;
}
