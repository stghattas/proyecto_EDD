#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

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
    TreeNode *left;  // Primogenito
    TreeNode *right; // Segundo hijo

    TreeNode(int id, const string &name, const string &last_name, char gender, int age, int id_father, bool is_dead, bool was_king, bool is_king)
        : id(id), name(name), last_name(last_name), gender(gender), age(age), id_father(id_father), is_dead(is_dead), was_king(was_king), is_king(is_king), left(nullptr), right(nullptr) {}
};

TreeNode* findNode(TreeNode* root, int id);

void printTree(TreeNode *root)
{
    if (!root)
        return;
    cout << "ID: " << root->id << ", Name: " << root->name << ", Age: " << root->age << ", Is King: " << root->is_king << endl;
    printTree(root->left);
    printTree(root->right);
}

// Funcion para insertar un nodo en el arbol binario
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
        // Evitar duplicados, pero esto no debería ocurrir si los IDs son unicos
        cerr << "Advertencia: Nodo duplicado con ID " << newNode->id << ".\n";
    }
    return root;
}

// Funcion para leer datos del archivo CSV y construir el arbol
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

    return root;
}

// Funcion para mostrar la linea de sucesion actual
void displaySuccessionLine(TreeNode *root)
{
    if (!root || root->is_dead)
        return;

    if (!root->is_dead)
        cout << root->name << " " << root->last_name << " (ID: " << root->id << ")\n";

    displaySuccessionLine(root->left);
    displaySuccessionLine(root->right);
}

// Funcion para asignar automaticamente el nuevo rey
TreeNode *assignNewKing(TreeNode *root)
{
    if (!root || root->is_dead)
        return nullptr;

    if (root->is_king && root->is_dead)
    {
        if (root->left && !root->left->is_dead)
        {
            root->left->is_king = true;
            return root->left;
        }
        else if (root->right && !root->right->is_dead)
        {
            root->right->is_king = true;
            return root->right;
        }
    }

    TreeNode *leftKing = assignNewKing(root->left);
    if (leftKing)
        return leftKing;

    TreeNode *rightKing = assignNewKing(root->right);
    if (rightKing)
        return rightKing;

    return nullptr;
}

// Funcion para modificar los datos de un nodo
void modifyNode(TreeNode *root)
{
    if (!root)
    {
        cout << "El arbol está vacío.\n";
        return;
    }

    int id;
    cout << "Ingrese el ID del nodo que desea modificar: ";
    cin >> id;

    TreeNode *node = findNode(root, id);
    if (!node)
    {
        cout << "No se encontro un nodo con el ID especificado.\n";
        return;
    }

    cout << "Datos actuales del nodo:\n";
    cout << "ID: " << node->id << "\n";
    cout << "Nombre: " << node->name << "\n";
    cout << "Apellido: " << node->last_name << "\n";
    cout << "Genero: " << node->gender << "\n";
    cout << "Edad: " << node->age << "\n";
    cout << "Está muerto: " << (node->is_dead ? "Si" : "No") << "\n";
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
    {
        node->gender = newGender[0];
    }

    cout << "Nueva edad: ";
    string newAge;
    getline(cin, newAge);
    if (!newAge.empty())
    {
        node->age = stoi(newAge);
    }

    cout << "Esta muerto? (1 para Si, 0 para No): ";
    string newIsDead;
    getline(cin, newIsDead);
    if (!newIsDead.empty())
    {
        node->is_dead = (newIsDead == "1");
    }

    cout << "Fue rey? (1 para Si, 0 para No): ";
    string newWasKing;
    getline(cin, newWasKing);
    if (!newWasKing.empty())
    {
        node->was_king = (newWasKing == "1");
    }

    cout << "Es rey? (1 para Si, 0 para No): ";
    string newIsKing;
    getline(cin, newIsKing);
    if (!newIsKing.empty())
    {
        node->is_king = (newIsKing == "1");
    }

    cout << "Datos actualizados correctamente.\n";
}

void showLineOfSuccession(TreeNode *root)
{
    if (!root)
        return;

    if (!root->is_dead)
    { // Solo mostrar los vivos
        cout << "ID: " << root->id << ", Name: " << root->name << ", Age: " << root->age << endl;
    }
    if (root->left)
        showLineOfSuccession(root->left);
    if (root->right)
        showLineOfSuccession(root->right);
}

TreeNode *findNode(TreeNode *root, int id)
{
    if (!root)
        return nullptr; // Si el arbol esta vacio, regresa nullptr

    if (root->id == id)
    {
        return root;
    }
    else if (id < root->id)
    {
        return findNode(root->left, id);
    }
    else
    {
        return findNode(root->right, id);
    }
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
                cout << "El árbol está vacío.\n";
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
