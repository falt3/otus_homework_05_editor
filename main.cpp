#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <sstream>


//----------------------------------------------------------------

class IPrimitive {
public:
    IPrimitive(const std::string& name) : m_name(name) { /*m_coord {0, 0};*/ }
    // void setCenter(int x, int y) {};
    virtual std::string print() = 0; //{ return "la-la-la"; }
protected:
    std::string m_name;
    // std::pair<int, int> m_coord;
};

class Circle : public IPrimitive {
public:
    Circle(const std::string& name, int radius) : IPrimitive(name), m_radius(radius) {}
    virtual std::string print() { 
        std::ostringstream ss;
        ss << "Circle: " << m_name << ", radius=" << m_radius;
        return ss.str(); 
    }
private:
    int m_radius;
};

class Rectangle : public IPrimitive {
public:
    Rectangle(const std::string& name, int width, int height) : IPrimitive(name), m_width(width), m_height(height) {}
    virtual std::string print() { 
        std::ostringstream ss;
        ss << "Rectangle: " << m_name << ", width=" << m_width << ", height=" << m_height;
        return ss.str(); 
    }
private:
    int m_width;
    int m_height;
};

//----------------------------------------------------------------

class Document {
public:
    Document(const std::string& name) : m_name(name) {}
    void addPrimitive(std::unique_ptr<IPrimitive> obj) {
        m_objects.push_back(std::move(obj));
    }
    void delPrimitive(std::string name) {}
    const std::string& name() { return m_name; }
    void print() {
         std::cout << "Документ: " << m_name << std::endl;
         for (auto& el : m_objects) {
            std::cout << "    " << el->print() << std::endl;
         }
    }
private:
    std::string m_name;
    std::list<std::unique_ptr<IPrimitive>> m_objects;
};

//----------------------------------------------------------------

class Model {
public:
    void newDocument(const std::string& name) { m_doc.reset(new Document(name)); };
    void newRectangle(const std::string& name, int width, int height) { 
        if (m_doc.get() != nullptr) { 
            Rectangle* r = new Rectangle(name, width, height);
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(r)); 
        } 
    };
    void newCircle(const std::string& name, int radius) {
        if (m_doc.get() != nullptr) { 
            Circle* c = new Circle(name, radius);
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(c)); 
        } 
    }

    void print() {
       if (m_doc.get() != nullptr)
        m_doc->print();
    };

private:
    std::unique_ptr<Document> m_doc;
    // Document dd     {"asd"};
};

//----------------------------------------------------------------

class View {
public:
    View() {};
    void view(Model& model) {
        model.print();
    };
private:
};

//----------------------------------------------------------------

class Controller {
public: 
    Controller(Model& model) : m_model(model) { std::cout << "Controller: constructor\n"; }
    void setView(View* view) { m_view = view; }
    ~Controller() { std::cout << "Controller: destructor\n"; }
    void execute();
private:
    void newDocument(std::string name) {};
    void openDocument(std::string name) {};
    void saveDocument(std::string name) {};

    Model& m_model;
    View* m_view;
};

void Controller::execute() {
    m_model.newDocument("Doc-1");
    m_model.newRectangle("Прямоугольник-1", 12, 13);
    m_model.newCircle("Окружность-1", 33);

    if (m_view != nullptr)
        m_view->view(m_model);
    // m_model.print();
}

//----------------------------------------------------------------

int main() 
{
    Model model;
    Controller controller(model);
    View viewer;
    // controller.setView(&viewer);


    controller.execute();

    // Rectangle rect("rect1", 12, 10);
    // std::cout << rect << std::endl;

    // Circle circle("asdald", 33);
    // std::cout << circle << std::endl;

    return 0;
}