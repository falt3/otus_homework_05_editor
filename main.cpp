#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <sstream>


//----------------------------------------------------------------
/**
 * @brief Интерфейс для графических примитивов
 * 
 */
class IPrimitive {
public:
    IPrimitive(int id) : m_id(id) {}
    virtual std::string print() = 0;
    int getId() { return m_id; }
protected:
    int m_id;       ///< Идентификатор примитива
};

/**
 * @brief Класс окружности
 * 
 */
class Circle : public IPrimitive {
public:
    Circle(int id, int radius) : IPrimitive(id), m_radius(radius) {}
    virtual std::string print() { 
        std::ostringstream ss;
        ss << "Circle: " <<  "id=" << m_id << ", radius=" << m_radius;
        return ss.str(); 
    }
private:
    int m_radius;   ///< Радиус окружности
};

/**
 * @brief Класс прямоугольника
 * 
 */
class Rectangle : public IPrimitive {
public:
    Rectangle(int id, int width, int height) : IPrimitive(id), m_width(width), m_height(height) {}
    virtual std::string print() { 
        std::ostringstream ss;
        ss << "Rectangle: " << "id=" << m_id << ", width=" << m_width << ", height=" << m_height;
        return ss.str(); 
    }
private:
    int m_width;        ///< Ширина прямоуголника
    int m_height;       ///< Высота прямоугольника
};

//----------------------------------------------------------------
/**
 * @brief Класс документа
 * @details Класс документа, содержит в себе имя документа и список графических примитивов
 * 
 */
class Document {
public:
    Document(const std::string& name) : m_name(name) { std::cout << "document: constructor (" << name << ")\n"; }
    ~Document() { std::cout << "document: destructor (" << m_name << ")\n"; }
    void addPrimitive(std::unique_ptr<IPrimitive> obj) { m_objects.push_back(std::move(obj)); }
    void delPrimitive(int id) { m_objects.remove_if([id](auto& el){ return (el->getId() == id); }); }
    const std::string& name() { return m_name; }
    void print() const {
         std::cout << "Документ: " << m_name << std::endl;
         for (auto& el : m_objects)
            std::cout << "    " << el->print() << std::endl;
    }
    int getId() { return ++m_id; }
private:
    std::string m_name;     ///< Имя документа
    std::list<std::unique_ptr<IPrimitive>> m_objects;   ///< Список графических примитивов
    int m_id  = 0;          ///< Счетчик для получения нового идентификатора 
};

//----------------------------------------------------------------
/**
 * @brief Класс модели данных для работы с документами
 * @details Класс модели данных для работы с документами, в данной реализации поддерживается только один открытый документ
 * 
 */
class Model {
public:
    void newDocument(const std::string& name) { m_doc.reset(new Document(name)); };
    void saveDocument() {};
    void openDocument(const std::string& name) {};

    void newRectangle(int width, int height) { 
        if (m_doc.get())
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(new Rectangle(m_doc->getId(), width, height))); 
    };
    void newCircle(int radius) {
        if (m_doc.get()) 
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(new Circle(m_doc->getId(), radius))); 
    }   
    void deleteObject(int id) { if (m_doc.get()) m_doc->delPrimitive(id); }
    void print() const { if (m_doc.get()) m_doc->print(); };
private:
    std::unique_ptr<Document> m_doc;    ///< текущий открытый документ
    bool m_editFlag;
};

//----------------------------------------------------------------

class Controller {
public: 
    Controller(Model& model) : m_model(model) { std::cout << "Controller: constructor\n"; }
    ~Controller() { std::cout << "Controller: destructor\n"; }
    void test1();
    void command(std::string line);
    const Model& getModel() {return m_model;}
private:
    // void newDocument(std::string name) {};
    // void openDocument(std::string name) {};
    // void saveDocument(std::string name) {};
    Model& m_model;
};

void Controller::test1() 
{
    m_model.newDocument("Doc-1");
    m_model.newRectangle(12, 13);
    m_model.newCircle(33);
    m_model.newCircle(124);
    m_model.print();
}

void Controller::command(std::string line) 
{
    if (line == "new document") {
        std::cout << "Введите имя документа: ";
        std::string ss_name;
        if (std::getline(std::cin, ss_name) && !ss_name.empty()) {
            m_model.newDocument(ss_name);
        }
    }
    else if (line == "open document") {
        std::cout << "Введите имя документа: ";
        std::string ss_name;
        if (std::getline(std::cin, ss_name) && !ss_name.empty()) {

        }
    }
    else if (line == "save document") {
        // m_model.
    }
    else if (line == "new rectangle") {
        
    }
    else if (line == "new circle") {
        std::cout << "Введите радиус окружности: ";
        int r   = 0;
        std::string ss_radius;
        if (std::getline(std::cin, ss_radius) && !ss_radius.empty()) {
            std::istringstream oss(ss_radius);
            oss >> r;
            m_model.newCircle(r);
        }
    }
    else if (line == "delete object") {
        std::cout << "Введите идентификатор объекта: ";
        int id = 0;
        std::string ss_id;
        if (std::getline(std::cin, ss_id) && !ss_id.empty()) {
            std::istringstream oss(ss_id);
            oss >> id;
            m_model.deleteObject(id);
        }
    }
    else {
        std::cout << "Нет такой команды\n";
    }
}

//----------------------------------------------------------------

class View {
public:
    View(Controller& controller) : m_controller(controller) {};
    void view(const Model& model) {
        model.print();
    };
    void execute();
private:
    Controller& m_controller;
};

void View::execute() {
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        m_controller.command(line);
    }
}

//----------------------------------------------------------------

int main() 
{
    Model model;
    Controller controller(model);
    View viewer(controller);
    controller.test1();

    // auto update = [](){ std::cout << "f_update\n"; };

    viewer.execute();

    viewer.view(controller.getModel());

    return 0;
}