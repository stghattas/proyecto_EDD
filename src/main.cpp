#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Códigos de color ANSI
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\e[0;31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define RED_HC "\e[0;91m"

// Nodo del arbol binario
struct TreeNode
{
    int id;
    string name;
    string last_name;
    char gender; // 'H' o 'M'
    int age;
    int id_father;
    bool is_dead;
    bool was_king;
    bool is_king;
    TreeNode *left;   // Primogenito
    TreeNode *right;  // Segundo hijo
    TreeNode *parent; // Padre del nodo

    TreeNode(int id, const string &name, const string &last_name, char gender, int age, int id_father, bool is_dead, bool was_king, bool is_king)
        : id(id), name(name), last_name(last_name), gender(gender), age(age), id_father(id_father), is_dead(is_dead), was_king(was_king), is_king(is_king), left(nullptr), right(nullptr), parent(nullptr) {}
};

// Buscar nodo por ID
TreeNode *findNode(TreeNode *root, int id)
{
    if (!root)
        return nullptr;

    if (root->id == id)
        return root;
    else if (id < root->id)
        return findNode(root->left, id);
    else
        return findNode(root->right, id);
}

TreeNode *findFirstAlivePrimogenito(TreeNode *root)
{
    if (!root)
        return nullptr;

    // Primero, buscar en el hijo izquierdo (primogénito)
    if (!root->is_dead)
    {
        return root;
    }

    // Si el primogénito está muerto, buscar recursivamente en sus hijos
    TreeNode *left = findFirstAlivePrimogenito(root->left);
    if (left)
        return left;

    // Si no se encuentra, buscar en el hijo derecho
    return findFirstAlivePrimogenito(root->right);
}

TreeNode *findFirstAliveSecondChild(TreeNode *root)
{
    if (!root)
        return nullptr;

    // Buscar en el segundo hijo
    if (!root->is_dead)
    {
        return root;
    }

    // Buscar en los descendientes del hijo derecho
    return findFirstAliveSecondChild(root->right);
}

TreeNode *findFirstAliveSibling(TreeNode *root)
{
    if (!root || !root->parent)
        return nullptr;

    // Buscar al hermano en el árbol del padre
    TreeNode *sibling = (root == root->parent->left) ? root->parent->right : root->parent->left;
    return findFirstAlivePrimogenito(sibling);
}

TreeNode *findAncestorWithTwoChildren(TreeNode *root)
{
    if (!root)
        return nullptr;

    // Si el ancestro tiene dos hijos vivos, devuelve el ancestro
    if (root->left && root->right && !root->left->is_dead && !root->right->is_dead)
    {
        return root;
    }

    // Recursivamente buscar en los padres
    return findAncestorWithTwoChildren(root->parent);
}

// Imprimir el arbol
void printTree(TreeNode *root)
{
    if (!root)
        return;

    // Imprimir cada campo en una linea separada
    cout << CYAN << BOLD << "ID: " << RESET << root->id << endl;
    cout << BOLD << "Nombre: " << RESET << root->name << endl;
    cout << BOLD << "Apellido: " << RESET << root->last_name << endl;
    cout << CYAN << BOLD << "Genero: " << RESET << (root->gender == 'M' ? "M" : "F") << endl;
    cout << CYAN << BOLD << "Edad: " << RESET << root->age << endl;
    cout << BOLD << RED << "ID Padre: " << RESET << root->id_father << endl;
    cout << BOLD << "Esta muerto: " << RESET << (root->is_dead ? GREEN "Sí" : RED_HC "No") << RESET << endl;
    cout << BOLD << "Fue Rey: " << RESET << (root->was_king ? GREEN "Si" : RED_HC "No") << RESET << endl;
    cout << BOLD << "Es Rey: " << RESET << (root->is_king ? GREEN "Si" : RED_HC "No") << RESET << endl;
    cout << endl;

    // Recursion sobre los hijos
    printTree(root->left);
    printTree(root->right);
}

// Insertar un nodo en el árbol binario
TreeNode *insertNode(TreeNode *root, TreeNode *newNode)
{
    if (!root)
        return newNode;

    if (newNode->id < root->id)
    {
        root->left = insertNode(root->left, newNode);
    }
    else if (newNode->id > root->id)
    {
        root->right = insertNode(root->right, newNode);
    }
    else
    {
        cerr << "Advertencia: Nodo duplicado con ID " << newNode->id << ".\n";
    }
    return root;
}

// Verificar y ajustar el rey en el arbol
void verifyKing(TreeNode *root)
{
    bool kingFound = false;
    if (!root)
        return;

    if (root->is_king)
    {
        if (kingFound)
        {
            root->is_king = false; // Si ya se encontro un rey, se marca este como no rey
        }
        else
        {
            kingFound = true; // Solo asignar el primer rey encontrado
        }
    }

    // Recursion sobre los hijos izquierdo y derecho
    verifyKing(root->left);
    verifyKing(root->right);
}

TreeNode *assignNewKing(TreeNode *root, TreeNode *currentKing)
{
    if (!root || !currentKing)
        return nullptr;

    // Si el rey actual tiene menos de 70 años y no esta muerto, no hacemos nada
    if (currentKing->age < 70 && !currentKing->is_dead)
    {
        cout << "El rey actual esta vivo y tiene menos de 70 anios.\n";
        return currentKing;
    }

    // Si el rey esta muerto, buscar a un sucesor
    cout << "El rey actual esta muerto o tiene más de 70 anios, buscando nuevo rey...\n";

    // 1. Buscar al primogénito vivo del arbol del rey
    if (currentKing->left)
    {
        TreeNode *newKing = findFirstAlivePrimogenito(currentKing->left);
        if (newKing)
        {
            newKing->is_king = true;
            cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
            return newKing;
        }
    }

    // 2. Si no tiene hijos, buscar al primogenito vivo del hermano
    TreeNode *siblingKing = findFirstAliveSibling(currentKing);
    if (siblingKing)
    {
        TreeNode *newKing = findFirstAlivePrimogenito(siblingKing);
        if (newKing)
        {
            newKing->is_king = true;
            cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
            return newKing;
        }
    }

    // 3. Si no tiene hijos ni hermanos, buscar al primogenito vivo del arbol del tio
    if (currentKing->parent && currentKing->parent->parent)
    {
        TreeNode *uncleKing = (currentKing->parent == currentKing->parent->parent->left)
                                  ? currentKing->parent->parent->right
                                  : currentKing->parent->parent->left;

        if (uncleKing)
        {
            TreeNode *newKing = findFirstAlivePrimogenito(uncleKing);
            if (newKing)
            {
                newKing->is_king = true;
                cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
                return newKing;
            }
        }
    }

    // 4. Si no se encuentra un rey, buscar el primer ancestro con dos hijos vivos
    TreeNode *ancestorKing = findAncestorWithTwoChildren(root);
    if (ancestorKing)
    {
        TreeNode *newKing = findFirstAlivePrimogenito(ancestorKing);
        if (newKing)
        {
            newKing->is_king = true;
            cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
            return newKing;
        }
    }

    // 5. Si todos los primogenitos están muertos, buscar los segundos hijos
    TreeNode *newKing = findFirstAliveSecondChild(currentKing->left);
    if (newKing)
    {
        newKing->is_king = true;
        cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
        return newKing;
    }

    newKing = findFirstAliveSecondChild(currentKing->right);
    if (newKing)
    {
        newKing->is_king = true;
        cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
        return newKing;
    }

    // Si no se pudo encontrar un rey, retornar nullptr
    cout << "No se pudo encontrar un nuevo rey.\n";
    return nullptr;
}

bool isKingExists(TreeNode *root)
{
    if (root == nullptr)
        return false;

    if (root->is_king)
    {
        return true;
    }

    // Recorrer de manera recursiva en los hijos izquierdo y derecho
    return isKingExists(root->left) || isKingExists(root->right);
}

// Buscar el rey actual
TreeNode *findKing(TreeNode *root)
{
    if (!root)
        return nullptr;
    if (root->is_king)
        return root; // Si este nodo es el rey, devolverlo
    TreeNode *leftResult = findKing(root->left);
    if (leftResult)
        return leftResult;        // Buscar recursivamente en el hijo izquierdo
    return findKing(root->right); // Buscar recursivamente en el hijo derecho
}

// Cargar datos desde el archivo CSV y construir el arbol
TreeNode *loadFromCSV(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo '" << filename << "'. Verifica la ruta y que el archivo exista.\n";
        return nullptr;
    }

    cout << "Archivo '" << filename << "' abierto correctamente.\n";

    string line;
    getline(file, line); // Leer la cabecera
    cout << "Cabecera del archivo: " << line << endl;

    TreeNode *root = nullptr;
    int successCount = 0, failureCount = 0;
    bool kingFound = false; // Bandera para verificar si ya hemos encontrado un rey valido

    while (getline(file, line))
    {
        cout << "Procesando linea: " << line << endl;
        stringstream ss(line);
        string token;
        int id, age, id_father;
        string name, last_name;
        char gender;
        bool is_dead, was_king, is_king;

        try
        {
            // Leer y procesar cada campo
            getline(ss, token, ',');
            id = stoi(token);

            getline(ss, token, ',');
            name = token;

            getline(ss, token, ',');
            last_name = token;

            getline(ss, token, ',');
            gender = token[0];

            getline(ss, token, ',');
            age = stoi(token);

            getline(ss, token, ',');
            id_father = stoi(token);

            getline(ss, token, ',');
            is_dead = (token == "true" || token == "1");

            getline(ss, token, ',');
            was_king = (token == "true" || token == "1");

            getline(ss, token, ',');
            is_king = (token == "true" || token == "1");

            // Si encontramos un rey con edad <= 70 y aun no hemos asignado uno
            if (!kingFound && is_king && age <= 70)
            {
                cout << "Rey encontrado con edad <= 70 años: " << name << " " << last_name << endl;
                kingFound = true; // Marcar que hemos encontrado un rey valido
            }
            else
            {
                is_king = false; // Marcar a los demas como no rey
            }

            // Crear el nuevo nodo
            TreeNode *newNode = new TreeNode(id, name, last_name, gender, age, id_father, is_dead, was_king, is_king);
            if (!root)
            {
                root = newNode;
            }
            else
            {
                root = insertNode(root, newNode);
            }

            successCount++;
        }
        catch (const exception &e)
        {
            cerr << "Error procesando linea: \"" << line << "\". Detalle: " << e.what() << "\n";
            failureCount++;
        }
    }

    file.close();
    cout << "Archivo procesado completamente.\n";
    cout << "Filas exitosas: " << successCount << ", Filas fallidas: " << failureCount << endl;

    // Verificar que haya solo un rey
    verifyKing(root);

    return root;
}

// Modificar los datos de un nodo (prohibido modificar id o id_father)
void modifyNode(TreeNode *root)
{
    if (!root)
    {
        cout << "El arbol está vacio.\n";
        return;
    }

    int id;
    cout << "Ingrese el ID del nodo que desea modificar: ";
    cin >> id;

    TreeNode *node = findNode(root, id);
    if (!node)
    {
        cout << "No se encontró un nodo con el ID especificado.\n";
        return;
    }

    // Buscar al rey actual
    TreeNode *currentKing = findKing(root); // Aqui encontramos al rey actual
    if (!currentKing)
    {
        cout << "No se encontró un rey actual.\n";
        return;
    }

    cout << "Datos actuales del nodo:\n";
    cout << "ID: " << node->id << "\n";
    cout << "Nombre: " << node->name << "\n";
    cout << "Apellido: " << node->last_name << "\n";
    cout << "Genero: " << node->gender << "\n";
    cout << "Edad: " << node->age << "\n";
    cout << "Esta muerto: " << (node->is_dead ? "Si" : "No") << "\n";
    cout << "Fue rey: " << (node->was_king ? "Si" : "No") << "\n";
    cout << "Es rey: " << (node->is_king ? "Si" : "No") << "\n";

    cout << "\nIngrese los nuevos valores (deje en blanco para no modificar):\n";

    cout << "Nuevo nombre: ";
    string newName;
    cin.ignore();
    getline(cin, newName);
    if (!newName.empty())
        node->name = newName;

    cout << "Nuevo apellido: ";
    string newLastName;
    getline(cin, newLastName);
    if (!newLastName.empty())
        node->last_name = newLastName;

    cout << "Nuevo genero (M/F): ";
    string newGender;
    getline(cin, newGender);
    if (!newGender.empty() && (newGender == "M" || newGender == "F"))
        node->gender = newGender[0];

    cout << "Nueva edad: ";
    string newAgeStr;
    getline(cin, newAgeStr);
    if (!newAgeStr.empty())
    {
        int newAge = stoi(newAgeStr);
        if (newAge >= 0)
            node->age = newAge;
    }

    cout << "Esta muerto? (0 para No, 1 para Si): ";
    bool isDead;
    cin >> isDead;
    cin.ignore();
    node->is_dead = isDead;

    if (isDead)
    {
        // Si el nodo se marca como muerto
        if (node->is_king)
        {
            node->was_king = true;
            node->is_king = false;
            cout << "El nodo estaba marcado como rey. Se ha actualizado como 'Fue rey'.\n";
        }

        cout << "Nodo marcado como muerto.\n";

        // Aquí asignamos un nuevo rey
        TreeNode *newKing = assignNewKing(root, currentKing); // Pasa el rey actual para reasignar el trono
        if (newKing)
        {
            cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
        }
    }
    else
    {
        // Si el nodo está vivo, permite modificar el estado de 'is_king' y 'was_king'
        cout << "El nodo esta vivo.\n";
        cout << "Fue rey? (0 para No, 1 para Si): ";
        bool wasKing;
        cin >> wasKing;
        node->was_king = wasKing;

        if (wasKing)
        {
            // Verificar si ya existe un rey en el árbol
            if (isKingExists(root))
            {
                // Si ya hay un rey, cancelar la selección y mostrar el mensaje
                node->is_king = false;
                cout << "Ya existe un rey, no pueden haber dos.\n";
            }
            else
            {
                // Si no hay rey, permitir elegir si el nodo es rey
                cout << "Es rey actualmente? (0 para No, 1 para Si): ";
                int isKing;
                cin >> isKing;
                node->is_king = (isKing == 1);
            }
        }
    }
}

// Mostrar la linea de sucesion con los detalles completos de cada miembro
void showLineOfSuccession(TreeNode *root)
{
    if (!root)
        return;

    // Mostrar solo a los miembros vivos
    if (!root->is_dead)
    {
        // Imprimir cada campo en su propia línea
        cout << CYAN << BOLD << "ID: " << RESET << root->id << endl;
        cout << BOLD << "Nombre: " << RESET << root->name << endl;
        cout << BOLD << "Apellido: " << RESET << root->last_name << endl;
        cout << CYAN << BOLD << "Genero: " << RESET << (root->gender == 'M' ? "M" : "F") << endl;
        cout << CYAN << BOLD << "Edad: " << RESET << root->age << endl;
        cout  <<BOLD << RED << "ID Padre: " << RESET << root->id_father << endl;
        cout << BOLD << "Esta muerto: " << RESET << (root->is_dead ? GREEN "Si" : RED_HC "No") << RESET << endl;
        cout << BOLD << "Fue Rey: " << RESET << (root->was_king ? GREEN "Si" : RED_HC "No") << RESET << endl;
        cout << BOLD << "Es Rey: " << RESET << (root->is_king ? GREEN "Si" : RED_HC "No") << RESET << endl;
        cout << endl; // Añadir una línea vacía para separar cada nodo
    }

    // Recorrer al primogenito
    showLineOfSuccession(root->left);
    // Recorrer al segundo hijo
    showLineOfSuccession(root->right);
}

void showMenu()
{
    cout << "1. Cargar datos desde archivo CSV\n";
    cout << "2. Mostrar el arbol actual\n";
    cout << "3. Mostrar linea de sucesion\n";
    cout << "4. Cambiar los datos de un nodo\n";
    cout << "5. Salir\n";
    cout << "Selecciona una opcion: ";
}

int main()
{
    TreeNode *root = nullptr;
    string filename = "./bin/datos.csv";

    while (true)
    {
        showMenu();
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            root = loadFromCSV(filename);
            if (root)
                cout << "Datos cargados correctamente.\n";
            else
                cout << "No se pudieron cargar los datos.\n";
            break;
        case 2:
            if (root)
            {
                cout << "Arbol actual:\n";
                printTree(root);
            }
            else
            {
                cout << "El arbol esta vacio.\n";
            }
            break;
        case 3:
            if (root)
            {
                cout << "Linea de sucesion actual:\n";
                showLineOfSuccession(root);
            }
            else
            {
                cout << "El arbol esta vacio.\n";
            }
            break;
        case 4:
            modifyNode(root);
            break;
        case 5:
            cout << "Saliendo.\n";
            return 0;
        default:
            cout << "Opcion no valida. Intentalo de nuevo.\n";
            break;
        }
    }
}
