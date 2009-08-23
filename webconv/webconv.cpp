// GNU GPL v3 or later
// @author Lasse Kärkkäinen / Tronic
// Compile with g++ webconv.cpp -o webconv

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stack>
using namespace std;

bool skipBOM(std::ifstream& f) {
	size_t pos = f.tellg();
	bool found = false;
	if (f.get() == 0xEF && f.get() == 0xBB && f.get() == 0xBF) found = true;
	f.seekg(pos, std::ios::beg);
	return found;
}

struct Page;

typedef std::vector<Page> Pagelist;

struct Page {
	Pagelist subpages;
	string title;
	string name;
	string link;
	bool generate;
	enum { UNINITIALIZED, UNSELECTED, SELECTED, PATH } selection;
	Page(string title = "", string name = "", bool generate = false): title(title), name(name), link(name), generate(generate), selection(UNINITIALIZED) {
		if (generate) link += ".html";
	}
};

class HTML {
	bool m_xhtml;
	std::string m_name;
	std::ofstream m_file;
	std::string m_indent;
	bool m_need_newline;
  public:

	HTML(std::string const& path, std::string const& name, bool xhtml = false):
	  m_xhtml(xhtml),
	  m_name(name),
	  m_file((path + name + (xhtml ? ".xhtml" : ".html")).c_str(), ios::out | ios::trunc),
	  m_need_newline(false)
	{
		
	}

	void set_indent(string const& indent) { m_indent = indent; }

	void indent() {
		m_indent += "  ";
	}
	
	void unindent() {
		size_t pos = m_indent.size() - 2;
		if (m_indent.substr(pos) != "  ") throw string("Internal error: invalid unindent");
		m_indent.erase(pos);
	}
	
	void newline() {
		m_file << '\n' << m_indent;
		m_need_newline = false;
	}

	void println(std::string const& line) {
		if (m_need_newline) newline();
		m_file << line;
		m_need_newline = true;
	}
	
	/** Print a menu out of a list of pages. **/
	void menu_process(Pagelist const& pl) {
		// Print the menu
		for (Pagelist::const_iterator it = pl.begin(); it != pl.end(); ++it) menu_entry(*it);
	}

	/** Print a menu entry for a single page (recursively including any subpages). **/	
	void menu_entry(Page const& page) {
		newline();
		switch (page.selection) {
		  case Page::UNINITIALIZED: throw string("Internal error: page selection uninitialized");
		  case Page::UNSELECTED: m_file << "<li>"; break;
		  case Page::SELECTED: m_file << "<li class=\"selected\">"; break;
		  case Page::PATH: m_file << "<li class=\"path\">"; break;
		}
		m_file << menu_title(page);
		menu_submenu(page);
		m_file << "</li>";
	}
	
	/** Print a submenu inside the <li> of an outer menu. **/
	void menu_submenu(Page const& page) {
		if (page.subpages.empty()) return;
		indent(); // for outer <li>
		newline(); m_file << "<ul>"; indent();
		menu_process(page.subpages);
		unindent(); newline(); m_file << "</ul>";
		unindent(); newline(); // for outer </li>
	}

	/** Format the title markup for a single page. **/
	std::string menu_title(Page const& page, bool useless_links = true) {
		if (page.title.empty()) {
			// No title => only print a <hr>
			return (m_xhtml ? "<hr />" : "<hr>");
		} else if (page.name.empty()) {
			// No link => just print the title with no link
			return "<h2>" + page.title + "</h2>";
		} else if (page.selection == Page::SELECTED) {
			// Selected page => dummy link or no link (comment out the one you don't need)
			if (useless_links) {
				return "<a href=\"#\">" + page.title + "</a>";
			} else {
				return page.title;
			}
		} else {
			// Normal link
			return "<a href=\"" + page.link + "\">" + page.title + "</a>";
		}
	}

	/** Format the path markup for the currently selected page. **/
	std::string path_process(Pagelist const& pl) {
		for (Pagelist::const_iterator it = pl.begin(); it != pl.end(); ++it) {
			if (it->selection == Page::UNSELECTED) continue;
			std::string tmp;
			if (it->selection == Page::PATH) tmp = " &gt; " + path_process(it->subpages);
			return menu_title(*it, false) + tmp;
		}
		return string();
	}

	/** Update menu selections. **/
	void menu_selections(Pagelist& pl) {
		for (Pagelist::iterator it = pl.begin(); it != pl.end(); ++it) menu_selection(*it, m_name);
	}

	bool menu_selection(Page& page, std::string const& name) {
		page.selection = Page::UNSELECTED;
		for (Pagelist::iterator it = page.subpages.begin(); it != page.subpages.end(); ++it) {
			if (menu_selection(*it, name)) {
				page.selection = Page::PATH;
				break;
			}
		}
		if (page.name == name) {
			page.selection = Page::SELECTED;
		}
		return (page.selection != Page::UNSELECTED);
	}
	
};

struct Block {
	string tag;
	string inner_tag;
	Block(string t, string i): tag(t), inner_tag(i) {}
};

class Constructor {
	string m_dir_source, m_dir_dest;
	Pagelist m_pages;

public:
	Constructor(string const& source, string const& dest): m_dir_source(source), m_dir_dest(dest) {
		if (source[source.size() - 1] != '/') m_dir_source += "/";
		if (dest[dest.size() - 1] != '/') m_dir_dest += "/";
	}

	/** Process <+BODY+> tag. **/
	void body_process(std::istream& in, HTML& document) {
		string line;
		std::stack<Block> blocks;
		while (in.good()) {
			// Eat tabs & count indent level
			int level = 0;
			{
				int ch;
				while ((ch = in.get()) == '\t') ++level;
				in.putback(ch);
			}
			if (!in.good()) break;
			// Read the rest of the line
			getline(in, line);
			// Skip empty/comment lines
			if (line.empty() || (line[0] == '#')) continue;
			// Find content separator
			string ctrl; // Control string (set of commands)
			string content;
			bool hasContent = false;
			if (line[0] == ':') {
				size_t pos = line.find(":", 1);
				if (pos != string::npos) {
					hasContent = true;
					content = line.substr(pos + 1);
					line.erase(pos);
				}
				ctrl = line.substr(1);
				line.clear();
			} else {
				hasContent = true;
				content = line;
			}
			enum { UNORDERED, ORDERED, BLOCK } list_type = UNORDERED;
			string tag = ((level == 0) ? "p" : blocks.top().inner_tag);
			string cl; // Class
			string blocktag; // Used when starting blocks
			while (ctrl.size()) {
				size_t pos = ctrl.find(',');
				string cmd;
				if (pos == string::npos) {
					cmd = ctrl; ctrl.clear();
				} else {
					cmd = ctrl.substr(0, pos); ctrl.erase(0, pos + 1);
				}
				if (cmd.size() == 0) throw string("Empty formatting command found");
				switch (cmd[0]) {
				  case '~':
					if (cmd.size() < 2) throw string("Invalid syntax for command '~'");
					cl = cmd.substr(1); break;
				  case 'h':
					if (cmd.size() != 2) throw string("Invalid syntax for command 'h'");
					if (!hasContent) throw string("Header without text");
					if (level == 0) tag = string("h") + cmd[1];
					else content = string("<h") + cmd[1] + ">" + content + "</h" + cmd[1] + ">";
					break;
				  case '>':
					if ((cmd.size() == 2) && (cmd[1] == '#')) list_type = ORDERED;
					else if ((cmd.size() == 2) && (cmd[1] == '*')) list_type = UNORDERED;
					else { list_type = BLOCK; blocktag = cmd.substr(1); }
					break;
				  default:
					throw string("Unknown formatting command: \"") + cmd + "\"";
				}
			}
			while (level < blocks.size()) {
				document.unindent();
				document.println("</" + blocks.top().tag + ">");
				blocks.pop();
			}
			while (level > blocks.size()) {
				switch (list_type) {
				  case UNORDERED: document.println("<ul>"); blocks.push(Block("ul", "li")); break;
				  case ORDERED: document.println("<ol>"); blocks.push(Block("ol", "li")); break;
				  case BLOCK: document.println("<" + blocktag + ">"); blocks.push(Block(blocktag, "")); break;
				}
				document.indent();
			}
			if (hasContent) {
				document.println('<' + tag + (cl.empty() ? "" : " class=\"" + cl + '"') + ">" + content + "</" + tag + '>');
			}
		}
		// Close any remaining blocks
		while (!blocks.empty()) {
			document.unindent();
			document.println("</" + blocks.top().tag + ">");
			blocks.pop();
		}
	}

	/** Generate an HTML page. **/
	void generate_page(Page const& page) {
		string name = page.name;
		string source = name + ".txt";
		ifstream in((m_dir_source + source).c_str(), ios::in);
		if (skipBOM(in)) std::cout << std::endl << "Warning: UTF-8 BOM found and ignored in " << source << std::endl;
		if (!in.good()) throw string("Unable to open ") + m_dir_source + source;
		string title;
		getline(in, title);
		if (title.size() == 0) title = "Untitled";
		string modelname = "Model.htm";
		while (1) {
			string line;
			getline(in, line);
			if (in.eof()) throw string("EOF while reading headers in ") + source;
			if (!in.good()) throw string("Unknown error reading ") + source;
			if (line.empty()) break;
			size_t pos = line.find('=');
			if (pos == string::npos) throw string("Invalid header line, missing '='.");
			string id = line.substr(0, pos);
			string val = line.substr(pos + 1);
			if (id == "model") modelname = val;
			else throw string("Unknown header id '") + id + "'.";
		}
		ifstream model((m_dir_source + modelname).c_str());
		skipBOM(model); // No warning here, just silently ignore it
		if (!model.good()) throw string("Unable to open ") + m_dir_source + modelname;
		HTML document(m_dir_dest, name);
		document.menu_selections(m_pages);
		// TODO: rewrite the parser
		while (model.good()) {
			string line;
			getline(model, line);
			size_t pos = line.find("<+MENU+>");
			if (pos != string::npos) {
				document.set_indent(line.substr(0, pos));
				// Print the menu
				document.menu_process(m_pages);
				document.set_indent("");
				continue;
			}
			pos = line.find("<+SUBMENU+>");
			if (pos != string::npos) {
				if (!page.subpages.empty()) {
					document.set_indent(line.substr(0, pos));
					document.println("<ul class=\"submenu\">");
					document.indent();
					// Print the menu
					document.menu_process(page.subpages);
					document.unindent();
					document.println("</ul>");
					document.set_indent("");
				}
				continue;
			}
			pos = line.find("<+BODY+>");
			if (pos != string::npos) {
				document.set_indent(line.substr(0, pos));
				size_t offset = in.tellg(); body_process(in, document); in.seekg(offset);
				document.set_indent("");
				continue;
			}
			pos = line.find("<+TITLE+>");
			if (pos != string::npos) line.replace(pos, 9, title);
			pos = line.find("<+PATH+>");
			if (pos != string::npos) line.replace(pos, 8, document.path_process(m_pages));
			document.println(line);
		}
		document.newline();
	}

	void process() {
		cout << "\x1B[1m>>>\x1B[0m Loading AWC-cfg.txt ... ";
		ifstream conf((m_dir_source + "AWC-cfg.txt").c_str());
		if (skipBOM(conf)) std::cout << std::endl << "Warning: UTF-8 BOM found and ignored" << std::endl;
		if (!conf.good()) {
			cout << "FAILED!" << endl;
			throw string("Unable to process without list of pages. Aborting.");
		}
		cout << endl;
		while (conf.good()) {
			string tmp;
			getline(conf, tmp);
			parse_menu(m_pages, tmp);
		}
		conf.close();
		cout << endl;
		cout << "\x1B[1m>>>\x1B[0m Processing pages ..." << endl << ' ';
		try {
			generate(m_pages);
		} catch (...) {
			cout << "->FAILED" << endl;
			throw;
		}
		cout << endl << endl;
		cout << "\x1B[1m>>>\x1B[0m ALL DONE" << endl << endl;
	}

	void parse_menu(Pagelist& pages, std::string record) {
		if (record.empty()) return; // throw string("Unexpected end of record");
		string name;
		string title;
		char type = record[0];
		record.erase(0, 1);
		switch (type) {
		  case '\t': // Indent
			if (pages.empty()) throw string("Invalid indentation");
			cout << "  ";
			parse_menu(pages.back().subpages, record);
			break;
		  case '#': // ## comments are printed on screen, otherwise ignored
			if (record[0] == '#') cout << "  \x1B[1;30m#" << record << "\x1B[0m" << endl;
		    break;
		  case '+': case '.': // A page, '+' means generated, '.' just for the menu (external link)
			{
				istringstream iss(record);
				getline(iss, name, ' ');
				getline(iss, title);
			}
			cout << "  \x1B[32m" << type << "\x1B[0m " << title << " (" << name << ")" << endl;
			pages.push_back(Page(title, name, (type == '+')));
			break;
		  case '-': // Separator line for the menu
			cout << "  ------ " << endl;
			pages.push_back(Page());
			break;
		  case '%': // Title entry for the menu
			{
				istringstream iss(record);
				if (iss.get() != ' ') throw string("No space after '%'");
				getline(iss, title);
			}
			cout << "  \x1B[1;32m%\x1B[0m " << title << endl;
			pages.push_back(Page(title));
			break;
		}
	}
	
	void generate(Pagelist& pages) {
		for (Pagelist::iterator it = pages.begin(); it != pages.end(); ++it) {
			if (it->generate) {
				cout << ' ' << it->name;
				generate_page(*it);
			}
			generate(it->subpages);
		}
	}
};

int main(int argc, char* args[]) {
	cout << "Accelerated Website Constructor 0.5" << endl << endl;
	if (argc != 3) { cout << "Usage: " << args[0] << " <source file dir> <destination website dir>" << endl; return -1; }
	try {
		Constructor c(args[1], args[2]);
		c.process();
	} catch (string e) {
		cout << endl << "\x1B[1;31mFATAL:\x1B[0m " << e << endl << endl;
		return 1;
	}
}

