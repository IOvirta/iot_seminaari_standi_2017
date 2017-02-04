#ifndef IOVIRTA_IOT_CONF_TREE_HPP
#define IOVIRTA_IOT_CONF_TREE_HPP

#include <string>
#include <vector>
#include <algorithm>


namespace iovirta_iot
{
namespace tree
{
template <typename KeyType = std::string, typename ValueType = std::string>
struct Node
{
    std::vector<Node<KeyType, ValueType>*> childs;
    Node<KeyType, ValueType> *parent;
    KeyType name;
    ValueType value;

    Node<KeyType, ValueType>() : Node<KeyType, ValueType>(KeyType{}, ValueType{}) {}
    Node<KeyType, ValueType>(KeyType key) : Node<KeyType, ValueType>(key) {}
    Node<KeyType, ValueType>(KeyType key, ValueType value) : Node<KeyType, ValueType>({}, key, value) {}
    Node<KeyType, ValueType>(Node<KeyType, ValueType> *parent, KeyType key, ValueType value) : parent(parent), name(key), value(value) {}
}; // struct Node

template <typename KeyType = std::string, typename ValueType = std::string>
class Tree
{
private:
    Node<KeyType, ValueType> *root_node_ = nullptr;

public:
    Tree<KeyType, ValueType>() : root_node_(new Node<KeyType, ValueType>(nullptr, {}, {})) {}
    ~Tree<KeyType, ValueType>()
    {
        delete_subtree(root_node_);
    }
    /* TODO: rule of 5 */

    Node<KeyType, ValueType> *add_node(KeyType key, ValueType value = {})
    {
        return add_node(root_node_, key, value);
    }
    Node<KeyType, ValueType> *add_node(Node<KeyType, ValueType> *root_node, KeyType key, ValueType value = {})
    {
        root_node->childs.push_back(new Node<KeyType, ValueType>(root_node, key, value));

        return root_node->childs.back();
    }

/*    std::vector<Node<KeyType, ValueType>*> get_childs() const
    {
        return get_childs(root_node_);
    }
    std::vector<Node<KeyType, ValueType>*> get_childs(Node<KeyType, ValueType> *node) const
    {}*/

    void delete_subtree(KeyType key)
    {
        delete_subtree(find(key));
    }
    void delete_subtree(Node<KeyType, ValueType> *node)
    {
        // poistaa Noden ja kaikki sen lapset
        /* TODO: parempi fixi */
        std::vector<Node<KeyType, ValueType>*> copy_c(node->childs.begin(), node->childs.end());
        for (auto it = copy_c.begin(); it != copy_c.end(); ++it)
            delete_subtree(*it);

        if (node->parent != nullptr)
        {
            auto parent_node = node->parent;

            parent_node->childs.erase(std::remove_if(parent_node->childs.begin(), parent_node->childs.end(), [node](auto other)
            {
                return node == other;
            }), parent_node->childs.end());
        }

        delete node;
    }

    bool has_child(Node<KeyType, ValueType> *node, KeyType name) const
    {
        for (auto child : node->childs)
            if (child->name == name)
                return true;

        return false;
    }

    Node<KeyType, ValueType> *get_child(Node<KeyType, ValueType> *node, KeyType name) const
    {
        for (auto child : node->childs)
            if (child->name == name)
                return child;

        throw std::runtime_error("Tree: invalid child");
    }

    ValueType get(Node<KeyType, ValueType> *node, KeyType name) const
    {
        if (has_child(node, name))
            return get_child(node, name)->value;
        if (node->name == name)
            return node->value;

        throw std::runtime_error("Tree: invalid path2");
    }

    template <typename... Path>
    ValueType get(Node<KeyType, ValueType> *node, KeyType name, Path... path) const
    {
        if (has_child(node, name))
            return get(get_child(node, name), path...);
        if (node->name == name)
            return get(node, path...);

        // error
        throw std::runtime_error("Tree: invalid path1");
    }

    /*ValueType get(KeyType name) const
    {
        if (root_node_->name == name)/** TODO **
            return root_node_->value;

        throw std::runtime_error("Tree: invalid path3");
    }*/

    template <typename... Path>
    ValueType get(KeyType name, Path... path) const
    {
        return get(root_node_, name, path...);
    }

    Node<KeyType, ValueType> *find_node(KeyType key, Node<KeyType, ValueType> *root_node = nullptr) const
    {
        if (root_node == nullptr)
            root_node = root_node_;

        // Lähdetään rootista, käytään kaikki lapset läpi, lasten lapset läpi, lasten lasten lapset....
        // kunnes löytyy node jonka nimi = key, jos ei löydy niin virhe
        for (auto it = root_node->childs.begin(); it != root_node_->childs.end(); ++it)
        {
            if ((*it)->name == key)
                return (*it);

            if ((*it)->childs.size() != 0)
            {
                auto found = find_node(key, (*it));

                if (found->name == key)
                    return found;
            }
        }

        throw std::runtime_error("Tree: key not found");
    }


}; // class Tree
} // namespace tree
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_CONF_TREE_HPP
