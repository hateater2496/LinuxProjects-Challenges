#include <iostream>
#include <memory>

struct TreeNode {
    int val;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

std::shared_ptr<TreeNode> lowestCommonAncestor(const std::shared_ptr<TreeNode>& root, const std::shared_ptr<TreeNode>& p, const std::shared_ptr<TreeNode>& q) {
    if (root == nullptr || root == p || root == q) {
        return root;
    }

    std::shared_ptr<TreeNode> left = lowestCommonAncestor(root->left, p, q);
    std::shared_ptr<TreeNode> right = lowestCommonAncestor(root->right, p, q);

    if (left != nullptr && right != nullptr) {
        return root;
    }

    return (left != nullptr) ? left : right;
}

int main() {
    // Create a binary tree
    std::shared_ptr<TreeNode> root = std::make_shared<TreeNode>(3);
    root->left = std::make_shared<TreeNode>(5);
    root->right = std::make_shared<TreeNode>(1);
    root->left->left = std::make_shared<TreeNode>(6);
    root->left->right = std::make_shared<TreeNode>(2);
    root->right->left = std::make_shared<TreeNode>(0);
    root->right->right = std::make_shared<TreeNode>(8);
    root->left->right->left = std::make_shared<TreeNode>(7);
    root->left->right->right = std::make_shared<TreeNode>(4);
    root->left->left->left = std::make_shared<TreeNode>(9);
    root->left->left->right = std::make_shared<TreeNode>(3);
    root->right->right->left = std::make_shared<TreeNode>(12);
    root->right->right->right = std::make_shared<TreeNode>(10);
    root->right->left->left = std::make_shared<TreeNode>(14);
    root->right->left->right = std::make_shared<TreeNode>(11);

    // Find the lowest common ancestor of nodes with values 7 and 4 (should be 2)
    std::shared_ptr<TreeNode> p = root->left->right->left;
    std::shared_ptr<TreeNode> q = root->left->right->right;
    std::shared_ptr<TreeNode> lca = lowestCommonAncestor(root, p, q);

    std::cout << "The lowest common ancestor is: " << lca->val << std::endl;

    // No need to manually clean up memory, smart pointers handle it automatically

    return 0;
}
