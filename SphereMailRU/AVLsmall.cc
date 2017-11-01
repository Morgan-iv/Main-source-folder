#include <string>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <set>

using namespace std;

struct AVLNode {
    AVLNode *left, *right; 
    string key;
    int height;
    AVLNode(string const &_key) : key(_key) {
       left = right = NULL;
       height = 1;
    }
    int getBalanceFactor() const {
        int r = right == NULL ? 0 : right->height;
        int l = left == NULL ? 0 : left->height;
        return r - l;
    }
    void fix() {
        int r = right == NULL ? 0 : right->height;
        int l = left == NULL ? 0 : left->height;
        height = (r > l ? r : l) + 1;
    }

     AVLNode *insert(string const &_key) {
         if (_key < key)
            left = left == NULL ? new AVLNode(_key) : left->insert(_key);
         else
            right = right == NULL ? new AVLNode(_key) : right->insert(_key);
         return balance();
     }
     AVLNode *findMinimum() {
        return left != NULL ? left->findMinimum() : this;
     }   

     AVLNode *removeMinimum() {
        if (left == NULL) return right;
        left = left->removeMinimum();
        return balance();
     }

     static AVLNode *remove(AVLNode *p, string const &_key) {
        if (p == NULL) return NULL;
        if (_key < p->key) {
           p->left = remove(p->left, _key);
           return p->balance();
        } else if (_key > p->key) {
           p->right = remove(p->right, _key);
           return p->balance();
        } else {
           AVLNode *l = p->left;
           AVLNode *r = p->right;
           delete p;
           if (r == NULL) return l;
           AVLNode *min = r->findMinimum();
           min->right = r->removeMinimum();
           min->left = l;
           return min->balance();
        }
    }
};

struct AVLTree {
    AVLNode *root;
    AVLTree() {
        root = NULL;
    }
    void print() const {
        if (root != NULL) root->print();
    }

    bool insert(string const &_key) {
        if (root == NULL) root = new AVLNode(_key);
        else root = root->insert(_key);
        return true;
    }

    bool remove(string const &_key) {
        root = AVLNode::remove(root, _key);
        return true;
    }
};
    
int main() {
    AVLTree t;
    t.insert("abra");
    t.insert("cadabra");
    t.insert("foo");
    t.insert("bar");
    t.print();
    t.remove("cadabra");
    t.print();
}
