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

// Buscar nodo por ID
TreeNode *findNode(TreeNode *root, int id);

// Imprimir el árbol
void printTree(TreeNode *root)
{
    if (!root)
        return;
    cout << "ID: " << root->id << ", Nombre: " << root->name << ", Apellido: " << root->last_name << ", Genero: " << root->gender << ", Edad: " << root->age << ", ID Padre: " << root->id_father << ", Esta muerto: " << root->is_dead << ", Fue rey: " << root->was_king << ", Es rey: " << root->is_king << endl;
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
            root->is_king = false;
        }
        else
        {
            kingFound = true;
        }
    }

    verifyKing(root->left);
    verifyKing(root->right);
}

// Asignar automaticamente un rey al miembro mas joven
void assignYoungestKing(TreeNode *root)
{
    if (!root)
        return;
    bool kingFound = false;
    TreeNode *youngest = nullptr;

    // Buscar al miembro más joven
    if (!root->is_dead && (!youngest || root->age < youngest->age))
    {
        youngest = root;
    }

    // Si no hay rey, asignar el mas joven
    if (!kingFound && youngest)
    {
        youngest->is_king = true;
        kingFound = true; // Asegurarse de que solo haya un rey
    }

    assignYoungestKing(root->left);
    assignYoungestKing(root->right);
}

// Asignar automaticamente un rey a la persona más joven
TreeNode *assignNewKing(TreeNode *root)
{
    if (!root)
        return nullptr;

    // Si el nodo actual es un candidato para rey, y no esta muerto
    if (!root->is_dead)
    {
        return root;
    }

    // Recorrer el árbol para encontrar al siguiente rey valido
    TreeNode *leftKing = assignNewKing(root->left);
    if (leftKing && !leftKing->is_dead)
        return leftKing;

    TreeNode *rightKing = assignNewKing(root->right);
    if (rightKing && !rightKing->is_dead)
        return rightKing;

    return nullptr; // Si no se encuentra un nuevo rey
}

bool isKingExists(TreeNode* root) {
    if (root == nullptr) return false;

    if (root->is_king) {
        return true;
    }

    // Recorrer de manera recursiva en los hijos izquierdo y derecho
    return isKingExists(root->left) || isKingExists(root->right);
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
    bool kingFound = false; // Bandera para verificar si ya hemos encontrado un rey

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

            // Si ya hemos encontrado un rey, no cargamos nodos con is_king == true
            if (is_king && kingFound)
            {
                cout << "Ya se ha encontrado un rey. Este nodo sera ignorado.\n";
                continue;
            }

            // Si encontramos un rey, marcamos la bandera kingFound
            if (is_king)
            {
                kingFound = true;
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

    // Asignar automaticamente un rey si no hay ninguno
    assignYoungestKing(root);

    return root;
}


// Modificar los datos de un nodo (prohibido modificar id o id_father)
void modifyNode(TreeNode *root)
{
    if (!root)
    {
        cout << "El arbol esta vacio.\n";
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

        // Verificar si se necesita asignar un nuevo rey
        TreeNode *newKing = assignNewKing(root);
        if (newKing)
        {
            cout << "Nuevo rey asignado: " << newKing->name << " " << newKing->last_name << endl;
        }
    }
    else
    {
        // Si el nodo esta vivo, permite modificar el estado de 'is_king' y 'was_king'
        cout << "El nodo esta vivo.\n";
        cout << "Fue rey? (0 para No, 1 para Si): ";
        bool wasKing;
        cin >> wasKing;
        node->was_king = wasKing;

        if (wasKing)
        {
            // Verificar si ya existe un rey en el arbol
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
                bool isKing;
                cin >> isKing;
                node->is_king = isKing;
            }
        }
        else
        {
            // Si no fue rey, se deja el valor de 'is_king' como falso
            node->is_king = false;
        }
    }

    cout << "Nodo actualizado correctamente.\n";
}


TreeNode *findNode(TreeNode *root, int id)
{
    if (!root)
        return nullptr;

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

// Mostrar la linea de sucesion con los detalles completos de cada miembro
void showLineOfSuccession(TreeNode *root)
{
    if (!root)
        return;

    // Mostrar solo a los miembros vivos
    if (!root->is_dead)
    {
        // Mostrar detalles completos del nodo
        cout << "ID: " << root->id
             << ", Name: " << root->name
             << ", Last Name: " << root->last_name
             << ", Gender: " << (root->gender == 'M' ? "M" : "F")
             << ", Age: " << root->age
             << ", ID Father: " << root->id_father
             << ", Is Dead: " << (root->is_dead ? "Yes" : "No")
             << ", Was King: " << (root->was_king ? "Yes" : "No")
             << ", Is King: " << (root->is_king ? "Yes" : "No") << endl;
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
