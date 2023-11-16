#include <iostream>
#include <queue>

class Node 
{
public:
    int key;
    Node* left;
    Node* right;

    Node(int key) 
    {
        this->key = key;
        left = nullptr;
        right = nullptr;
    }
};

class BST 
{
public:
    Node* root;

    BST() // Конструкторы
    {
        root = nullptr;
    }

    
    BST(int key) 
    {
        root = new Node(key);
    }

    BST(Node* node) 
    {
        root = node;
    }

    BST(BST& bst) 
    {
        root = bst.root;
    }

   
    ~BST()  // Деструктор
    {
        destroy(root);
    }

    
    BST& operator=(BST& bst) // Оператор =
    {
        if (this != &bst) 
        {
            destroy(root);
            root = bst.root;
        }
        return *this;
    }


    Node* search(int key) 
    {
        return search(root, key);
    }

    
    void insert(int key) // Метод добавления узла в дерево
    {
        root = insert(root, key);
    }

    
    void remove(int key) // Метод удаления узла из дерева
    {
        root = remove(root, key);
    }

    Node* findMin() // Поиск узла с минимальным ключом
    {
        if (root == nullptr) 
        {
            return nullptr;
        }
        Node* current = root;
        while (current->left != nullptr) 
        {
            current = current->left;
        }
        return current;
    }

    
    Node* findMax() // Поиск узла с максимальным ключом
    {
        if (root == nullptr) 
        {
            return nullptr;
        }
        Node* current = root;
        while (current->right != nullptr) 
        {
            current = current->right;
        }
        return current;
    }

    
    void printTree() const // Вывод дерева на экран
    {
        std::cout << "Дерево:" << std::endl;
        printTree(root, 0);
    }

    void printLevelOrder() {
        if (root == nullptr) {
            return;
        }

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                Node* currentNode = q.front();
                q.pop();
                std::cout << currentNode->key << " ";

                if (currentNode->left != nullptr) {
                    q.push(currentNode->left);
                }
                if (currentNode->right != nullptr) {
                    q.push(currentNode->right);
                }
            }
            std::cout << std::endl;
        }
    }

    void inorderTraversal(Node* node)   //обход ЛПК
    {
        if (node == nullptr)
        {
            return;
        }
        inorderTraversal(node->left);
        std::cout << node->key << " ";
        inorderTraversal(node->right);
    }

    

private:
    
    void destroy(Node* node) // Вспомогательная функция для удаления всех узлов дерева
    {
        if (node != nullptr) 
        {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    Node* search(Node* node, int key) 
    {
        if (node == nullptr || node->key == key) 
        {
            return node;
        }
        if (node->key > key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

   
    Node* insert(Node* node, int key)  // Вспомогательная рекурсивная функция для добавления узла в дерево
    {
        if (node == nullptr) 
        {
            return new Node(key);
        }
        if (key < node->key) 
        {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) 
        {
            node->right = insert(node->right, key);
        }
        return node;
    }


    Node* remove(Node* node, int key)    // Вспомогательная рекурсивная функция для удаления узла из дерева
    {
        if (node == nullptr) 
        {
            return nullptr;
        }
        if (key < node->key) 
        {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else 
        {
            if (node->left == nullptr) 
            {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) 
            {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = remove(node->right, temp->key);
        }
        return node;
    }

    Node* minValueNode(Node* node)
    {
        Node* current = node;
        while (current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }



    void printTree(const Node* node, int level) const    // Вспомогательная функция для вывода дерева на экран
    {
        if (node == nullptr) 
        {
            return;
        }
        printTree(node->right, level + 1);
        for (int i = 0; i < level; i++) 
        {
            std::cout << "   ";
        }
        std::cout << node->key << std::endl;
        printTree(node->left, level + 1);
    }

   
};

int main() 
{
    setlocale(LC_ALL, "Russian");
    
    BST bst(10);    // Создаем новое дерево

    
    bst.insert(5);  // Добавляем узлы
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    bst.insert(13);
    bst.insert(17);

    
    bst.printTree();    // Выводим дерево на экран


    std::cout << "ЛПК обход: " << std::endl;    // Обходим дерево в порядке возрастания ключей
    bst.inorderTraversal(bst.root);    //обход ЛПК

    std::cout << "Обход по уровням: " << std::endl;    //обход по уровням
    bst.printLevelOrder();
    std::cout << std::endl;


    Node* node = bst.search(7);    // Проверяем поиск узлов
    if (node != nullptr) 
    {
        std::cout << "Узел с ключом 7 найден!" << std::endl;
    }
    else {
        std::cout << "Узел с ключом 7 не найден :(" << std::endl;
    }


    bst.remove(15);    // Удаляем узел


    Node* minNode = bst.findMin();    // Проверяем минимальный и максимальный узлы
    Node* maxNode = bst.findMax();
    std::cout << "Минимальный ключ: " << minNode->key << std::endl;
    std::cout << "Максимальный ключ: " << maxNode->key << std::endl;



    
    return 0;
}
