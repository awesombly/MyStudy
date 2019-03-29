#include "header.h"

using namespace std;

class Graphics {
public:
	Graphics() {};
	~Graphics() {};

	virtual void Acting() {
		cout << __FUNCTION__ << endl;
	};
	virtual void AddSome(Graphics* grap) {};
private:

};

class Line : public Graphics {
public:
	Line() {};
	~Line() {};

	virtual void Acting() override 
	{
		cout << __FUNCTION__ << endl;
	};
private:
};

class Rect : public Graphics {
public:
	Rect() {};
	~Rect() {};

	virtual void Acting() override
	{
		cout << __FUNCTION__ << endl;
	};
private:
};

class Composite : public Graphics {
public:
	Composite() {};
	~Composite() {};

	virtual void Acting() override
	{
		cout << __FUNCTION__ << endl;
	};
	virtual void AddSome(Graphics* grap) override 
	{
		m_list.push_back(grap);
	}
private:
	list<Graphics*> m_list;
};

class Palette {
public:
	Palette() {};
	~Palette() {};

	void AddGraphics(string name, Graphics* grap)
	{
		m_map[name] = grap;
	}
	map<string, Graphics*>& getGraphics()
	{
		return m_map;
	}
private:
	map<string, Graphics*> m_map;
};



void main()
{
	Graphics* line = new Line();
	Graphics* rect = new Rect();
	Graphics* composite = new Composite();

	composite->AddSome(new Line());
	composite->AddSome(new Rect());
	composite->AddSome(new Line());

	Palette palette;
	palette.AddGraphics("Line", line);
	palette.AddGraphics("Rect", rect);
	palette.AddGraphics("Composite", composite);

	auto& grap = palette.getGraphics();
	for (auto iter : grap)
	{
		iter.second->Acting();
	}
	while (1);
}