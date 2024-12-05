Proyecto de Estructura Dinamica de Datos - URU
Integrantes:
- Samer Ghattas, C.I: 31.887.714

🔮👑Proyecto de Árbol Genealógico de Reyes

Este proyecto simula un sistema para gestionar un árbol genealógico de reyes, donde se controla el rol de rey en función de ciertas reglas (edad, muerte, sucesión). El programa permite modificar la información de los individuos dentro del árbol, actualizar su estado de "rey" y "muerto", y asignar un nuevo rey siguiendo una serie de criterios.

📝 Descripción del Proyecto:

El código se organiza en una estructura de árbol binario donde cada nodo representa un individuo. Cada nodo tiene información acerca del individuo, como su nombre, apellido, edad, si está muerto o vivo, y si ha sido rey o es actualmente rey.

🏗️ Funciones principales del programa:

Verificación de rey único: Asegura que solo haya un rey en el árbol, desmarcando automáticamente a otros si se encuentra más de uno.

Asignación de un nuevo rey: Si el rey actual ha muerto o tiene más de 70 años, se asigna un nuevo rey de acuerdo con las reglas predefinidas (buscar al primogénito vivo, o al hermano del rey, etc.).

Modificación de nodos: Permite modificar los atributos de los individuos (nombre, edad, estado de muerte) y actualiza automáticamente el estado de "rey" cuando es necesario.

Gestión del estado de muerte: Si un nodo es marcado como muerto, el sistema busca un nuevo rey si el nodo muerto era un rey.

📒 Reglas para la Asignación de un Rey:

El rey actual no debe tener más de 70 años ni estar muerto.

Si el rey está muerto o tiene más de 70 años, se buscará un sucesor.


❗ El sucesor debe cumplir con los siguientes criterios:

Primero se busca al primogénito vivo del árbol del rey.

Si no tiene primogénitos, se busca a su hermano (sibling).

Si tampoco tiene hermano, se busca al primogénito del tío del rey.

Si todos los primogénitos están muertos, se busca el primer hijo vivo de los descendientes.

🏭 Estructura del Proyecto:

El proyecto sigue un diseño basado en estructuras de datos y árboles binarios. Cada nodo en el árbol tiene la siguiente estructura:

" struct TreeNode {
    int id;
    string name;
    string last_name;
    char gender;  // 'M' para masculino, 'F' para femenino
    int age;
    bool is_dead;  // Estado de muerte del individuo
    bool was_king; // Indica si el individuo fue rey
    bool is_king;  // Indica si el individuo es rey actualmente
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
}; "

❓ Cómo Ejecutar el Código:

1. Clona el repositorio: " git clone https://github.com/stghattas/proyecto_EDD.git "

2. Compilar el código: " g++ -o .\bin\program.exe .\src\main.cpp "

3. Ejecutar el programa: " .\bin\program.exe "

❗ NOTA ❗

Recordar estar bien situado en el Visual Studio Code en cuanto a carpetas, normalmente el codigo no deberia de dar ningun fallo pero siempre es mejor un doble chequeo.
