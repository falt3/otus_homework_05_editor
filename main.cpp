#include <iostream>
#include <string>
#include <memory>
#include <list>


//----------------------------------------------------------------

class IPrimitive {
public:
    IPrimitive(const std::string& name) : m_name(name) { /*m_coord {0, 0};*/ }
    // void setCenter(int x, int y) {};
    // friend std::ostream& operator<< (std::ostream& out, const IPrimitive& a) {};
protected:
    std::string m_name;
    // std::pair<int, int> m_coord;
};

class Circle : public IPrimitive {
public:
    Circle(const std::string& name, int radius) : IPrimitive(name), m_radius(radius) {}
    friend std::ostream& operator<< (std::ostream& out, const Circle& a) {
        out << "Circle:" << " name="<< a.m_name << ", radius=" << a.m_radius;
        return out;
    }
private:
    int m_radius;
};

class Rectangle : public IPrimitive {
public:
    Rectangle(const std::string& name, int width, int height) : IPrimitive(name), m_width(width), m_height(height) {}
    friend std::ostream& operator<< (std::ostream& out, const Rectangle& a) {
        out << "Rectangle:" << " name="<< a.m_name << ", width=" << a.m_width << ", height=" << a.m_height;
        return out;
    }
private:
    int m_width;
    int m_height;
};

//----------------------------------------------------------------

class Document {
public:
    Document(const std::string& name) : m_name(name) {}
    void addPrimitive(std::unique_ptr<IPrimitive> obj) {}
    void delPrimitive(std::string name) {}
    const std::string& name() { return m_name; }
private:
    std::string m_name;
    std::list<std::unique_ptr<IPrimitive>> m_objects;
};

//----------------------------------------------------------------

class Model {
public:
    void newDocument(const std::string& name) { m_doc.reset(new Document("name")); };
    void newRectangle(const std::string& name, int width, int height) { 
        if (m_doc.get() != nullptr) { 
            Rectangle* r = new Rectangle(name, width, height);
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(r)); 
            // dd.addPrimitive(std::unique_ptr<IPrimitive>(r));
        } 
            // BlockCommand *cmd2 = new BlockCommand();
            // dinamicBlock(*cmd2, in);
            // if (cmd2->getCountCommands() > 0)
            //     cmd.addCommand(std::unique_ptr<ICommand>(cmd2));        
    };
private:
    std::unique_ptr<Document> m_doc;
    // Document dd     {"asd"};
};

//----------------------------------------------------------------

class Viewer {
public:
    Viewer() = default;
};

//----------------------------------------------------------------

class Controller {
public: 
    Controller(Model& model) : m_model(model) { std::cout << "Controller: constructor\n"; }
    ~Controller() { std::cout << "Controller: destructor\n"; }
    void execute();
private:
    void newDocument(std::string name) {};
    void openDocument(std::string name) {};
    void saveDocument(std::string name) {};

    Model& m_model;
};

void Controller::execute() {

}

//----------------------------------------------------------------

int main() 
{
    Model model;
    Controller controller(model);
    // Viewer viewer;

    // Rectangle rect("rect1", 12, 10);
    // std::cout << rect << std::endl;

    // Circle circle("asdald", 33);
    // std::cout << circle << std::endl;

    return 0;
}