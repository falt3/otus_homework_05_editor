/**
 * @file main.cpp
 * @author Lipatkin Dmitry
 * @brief 
 * @version 0.1
 * @date 2023-09-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <functional>
#include <fstream>

//----------------------------------------------------------------
/**
 * @brief Интерфейс для графических примитивов
 * 
 */
class IPrimitive {
public:
    IPrimitive(int id) : m_id(id) {}
    virtual std::string print() = 0;
    virtual std::string printSave() = 0;
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
    std::string print() { 
        std::ostringstream ss;
        ss << "Circle: " <<  "id=" << m_id << ", radius=" << m_radius;
        return ss.str(); 
    }
    std::string printSave() {
        std::ostringstream ss;
        ss << "circle " <<  m_id << " " << m_radius;
        return ss.str(); 
    };
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
    std::string print() { 
        std::ostringstream ss;
        ss << "Rectangle: " << "id=" << m_id << ", width=" << m_width << ", height=" << m_height;
        return ss.str(); 
    }
    std::string printSave() {
        std::ostringstream ss;
        ss << "rectangle " <<  m_id << " " << m_width << " " << m_height;
        return ss.str(); 
    };    
private:
    int m_width;        ///< Ширина прямоуголника
    int m_height;       ///< Высота прямоугольника
};

//----------------------------------------------------------------
/**
 * @brief Класс документа
 * @details Документ содержит в себе имя документа и список графических примитивов
 * 
 */
class Document {
public:
    Document(const std::string& name) : m_name(name) {}
    void addPrimitive(std::unique_ptr<IPrimitive> obj) { m_objects.push_back(std::move(obj)); }
    void delPrimitive(int id) { m_objects.remove_if([id] (auto& el) { return (el->getId() == id); }); }
    const std::string& name() { return m_name; }
    void print() const {
         std::cout << "Документ: " << m_name << std::endl;
         for (auto& el : m_objects)
            std::cout << "    " << el->print() << std::endl;
    }
    int getId() { return ++m_id; }

    friend class Memento;
private:
    std::string m_name;     ///< Имя документа
    std::list<std::unique_ptr<IPrimitive>> m_objects;   ///< Список графических примитивов
    int m_id  = 0;          ///< Счетчик для получения нового идентификатора 
};

//----------------------------------------------------------------
/**
 * @brief Класс, отвечающий за сохранение и открытие документа
 * 
 */
class Memento {
public:
    static void saveDocument(const Document& doc);
    static bool openDocument(const std::string& name, Document& doc);
};

void Memento::saveDocument(const Document& doc) 
{
    std::fstream fs(doc.m_name, std::ios::out);
    fs << "document " << doc.m_id << std::endl; 
    if (fs.is_open()) {
        for (auto& el : doc.m_objects) {
            fs << el->printSave() << std::endl;
        }
        fs.close();
    }
}

bool Memento::openDocument(const std::string& name, Document& doc) 
{
    std::fstream fs(name, std::ios::in);
    if (!fs.is_open()) return false;

    std::string line;
    while (std::getline(fs, line) && !line.empty()) {
        std::istringstream iss(line);
        std::string ss;
        iss >> ss;
        if (ss == "document") {
            int id = 0;
            iss >> id;
            doc.m_id = id;
        }
        else if (ss == "circle") {
            int id, r = 0;
            iss >> id >> r;
            if (r != 0) {
                doc.addPrimitive(std::unique_ptr<IPrimitive>(new Circle(id, r)));
            }
        }
        else if (ss == "rectangle") {
            int id, w, h;
            iss >> id >> w >> h;
            if (h != 0) {
                doc.addPrimitive(std::unique_ptr<IPrimitive>(new Rectangle(id, w, h)));
            }
        }        
    }
    fs.close();
    return true;
}

//----------------------------------------------------------------
/**
 * @brief Класс подписчика сообщений
 * 
 */
class Subscriber {
public:    
    virtual ~Subscriber() = default;
    virtual void update() = 0;
};
/**
 * @brief Класс издателя сообщений
 * 
 */
class Publisher {
public:
    void addSubscriber(std::shared_ptr<Subscriber> ss) {
        m_subscribers.push_back(ss);
    }
protected:
    void notify() {
        for (auto& el : m_subscribers)
            el->update();
    };
private:
    std::list<std::shared_ptr<Subscriber>> m_subscribers;
};

//----------------------------------------------------------------
/**
 * @brief Класс модели данных для работы с документами
 * @details Класс модели данных для работы с документами, в данной реализации поддерживается только один открытый документ
 * 
 */
class Model : public Publisher {
public:
    void newDocument(const std::string& name) { 
        m_doc.reset(new Document(name));  
        notify();
    };
    void saveDocument() { 
        if (m_doc.get()) 
            Memento::saveDocument(*m_doc);
    }
    void openDocument(const std::string& name) {
        std::unique_ptr<Document> doc(new Document(name));
        if (Memento::openDocument(name, *doc)) {
            m_doc.swap(doc);
            notify();
        }
    };
    void newRectangle(int width, int height) { 
        if (m_doc.get()) {
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(new Rectangle(m_doc->getId(), width, height))); 
            notify();
        }
    };
    void newCircle(int radius) {
        if (m_doc.get()) {
            m_doc->addPrimitive(std::unique_ptr<IPrimitive>(new Circle(m_doc->getId(), radius))); 
            notify();
        }
    }   
    void deleteObject(int id) { 
        if (m_doc.get()) {
            m_doc->delPrimitive(id); 
            notify();
        }
    }
    void print() const { 
        if (m_doc.get()) 
            m_doc->print(); 
    };    
private:
    std::unique_ptr<Document> m_doc;    ///< текущий открытый документ
};

//----------------------------------------------------------------
/**
 * @brief Класс обеспечивающий управление Моделью
 * 
 */
class Controller {
public: 
    Controller(Model& model) : m_model(model) {}
    void command(std::string line);
    const Model& model() { return m_model; }
private:
    Model& m_model;
};

void Controller::command(std::string line) 
{
    if (line == "new document") {
        std::cout << "Введите имя документа: ";
        std::string ss_name;
        if (std::getline(std::cin, ss_name) && !ss_name.empty()) 
            m_model.newDocument(ss_name);
    }
    else if (line == "open document") {
        std::cout << "Введите имя документа: ";
        std::string ss_name;
        if (std::getline(std::cin, ss_name) && !ss_name.empty())
            m_model.openDocument(ss_name);
    }
    else if (line == "save document") {
        m_model.saveDocument();
    }
    else if (line == "new rectangle") {
        std::cout << "Введите ширину прямоугольника: ";
        int w = 0;
        std::string ss_width;
        if (std::getline(std::cin, ss_width) && !ss_width.empty()) {
            std::istringstream oss(ss_width);
            oss >> w;
        } 
        if (w > 0) {
            std::cout << "Введите длину прямоугольника: ";
            int h = 0;
            std::string ss_height;
            if (std::getline(std::cin, ss_height) && !ss_height.empty()) {
                std::istringstream oss(ss_height);
                oss >> h;
            }       
            if (h > 0)
                m_model.newRectangle(w, h);
            else std::cout << "Ошибка ввода\n";
        }
        else std::cout << "Ошибка ввода\n";
    }
    else if (line == "new circle") {
        std::cout << "Введите радиус окружности: ";
        int r   = 0;
        std::string ss_radius;
        if (std::getline(std::cin, ss_radius) && !ss_radius.empty()) {
            std::istringstream oss(ss_radius);
            oss >> r;
            if (r != 0)
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
/**
 * @brief Класс представления модели пользователю
 * 
 */
class View : public Subscriber {
public:
    View(Controller& controller) : m_controller(controller) {};
    void execute();
    void update() override { 
        m_controller.model().print();
    };
private:
    Controller& m_controller;
};

void View::execute() {
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line) && !line.empty()) {
        m_controller.command(line);
    }
}

//----------------------------------------------------------------

int main() 
{
    Model model;
    Controller controller(model);
    auto viewer = std::make_shared<View>(controller);
    model.addSubscriber(viewer);

    viewer->execute();

    return 0;
}