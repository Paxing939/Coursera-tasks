#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <vector>

template<typename T>
class Tree {
public:
    Tree() : size_(0), sum_(0), root_(nullptr) {}

    long long GetSum() const { return sum_; }

    void Add(T element) {
        Node *node = root_;
        if (root_ == nullptr) {
            root_ = new Node;
            root_->key_ = element;
            root_->left_ = nullptr;
            root_->right_ = nullptr;
            size_++;
            sum_ += element;
            return;
        }

        while (true) {
            if (node->key_ > element) {
                if (node->left_ == nullptr) {
                    Node *tmp = new Node(element);
                    size_++;
                    sum_ += element;
                    node->left_ = tmp;
                    node = node->left_;
                    return;
                }
                node = node->left_;
                continue;
            }
            if (node->key_ < element) {
                if (node->right_ == nullptr) {
                    Node *tmp = new Node(element);
                    size_++;
                    sum_ += element;
                    node->right_ = tmp;
                    node = node->right_;
                    return;
                }
                node = node->right_;
                continue;
            }
            if (node->key_ == element) {
                return;
            }
        }

    }

    void Delete(T element) {
        Node *node = root_;

        if (root_->key_ == element) {
            root_ = DeleteNode(root_);
            return;
        }

        while (true) {
            if (node == nullptr) { return; }

            if (node->key_ > element) {
                if (node->left_->key_ == element) {
                    node->left_ = DeleteNode(node->left_);
                    return;
                }
                node = node->left_;
                continue;
            }
            if (node->key_ < element) {
                if (node->right_->key_ == element) {
                    node->right_ = DeleteNode(node->right_);
                    return;
                }
                node = node->right_;
                continue;
            }
        }

    }

    void PrintSum(std::ostream& out) {
        out << sum_;
    }

    void PrintTree(std::string file_name) {
        writer.open(file_name);
        NLRForFile(root_);
    }

    void PrintTree(std::ostream& out) {
        NLRWithVector(root_);

        for (const auto& key : elements_) {
            out << key << '\n';
        }
    }

private:
    struct Node {
        Node() {}

        Node (T element) {
            key_ = element;
            left_ = nullptr, right_ = nullptr;
        }

        T key_;
        Node *left_, *right_;
    };

    void NLRWithVector(Node *node) {
        if (node == nullptr) {
            return;
        }
        elements_.push_back(node->key_);
        NLRWithVector(node->left_);
        NLRWithVector(node->right_);
    }

    void NLRForFile(Node *node) {
        if (node == nullptr) {
            return;
        }
        writer << node->key_ << '\n';
        NLRForFile(node->left_);
        NLRForFile(node->right_);
    }

    Node* DeleteNode(Node *node) {
        if (node->left_ == nullptr) {
            if (node->right_ == nullptr) {
                return nullptr;
            } else {
                return node->right_;
            }
        } else {
            if (node->right_ == nullptr) {
                return node->left_;
            }
        }

        if (node->right_->left_ == nullptr) {
            node->right_->left_ = node->left_;
            return node->right_;
        }

        Node *tmp = node->right_;
        while (true) {
            if (tmp->left_->left_ == nullptr) {
                break;
            }
            tmp = tmp->left_;
        }
        node->key_ = tmp->left_->key_;
        tmp->left_ = tmp->left_->right_;
        return node;
    }

    std::vector<T> elements_;
    std::ofstream writer;
    Node *root_;
    long long size_, sum_;
};

int main() {
    std::ifstream reader("input.txt");
    Tree<int> tree;

    int number = 0, number_to_delete = 0;
    reader >> number_to_delete;

    while (!reader.eof()) {
        reader >> number;
        tree.Add(number);
    }

    //std::ofstream writer("output.txt");
    //tree.PrintSum(writer);
    //std::cout << "Sum is " << tree.GetSum() << '\n';
//    tree.PrintTree(std::cout);

    tree.Delete(number_to_delete);
//    tree.PrintTree(std::cout);
    tree.PrintTree("output.txt");

    return 0;
}