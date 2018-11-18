#include <iostream>
#include <utility>
#include <cstdlib>
#include <ctime>

using namespace std;


struct Node
{
	int value;
	int priority;
	int size;

	Node* left;
	Node* right;

	Node(int value)
		: value(value), priority(rand()), size(1), left(nullptr), right(nullptr)
	{

	}

	~Node()
	{
		if (left != nullptr) delete left;
		if (right != nullptr) delete right;
	}
};

int get_size(Node* root)
{
	if (root == nullptr) return 0;
	return root->size;
}

int get_priority(Node* root)
{
	if (root == nullptr) return 0;
	return root->priority;
}

void relax(Node* root)
{
	if (root == nullptr) return;
	root->size = get_size(root->left) + get_size(root->right) + 1;
}

Node* merge(Node* first, Node* second)
{
	if (first == nullptr) return second;
	if (second == nullptr) return first;

	if (get_priority(first) > get_priority(second))
	{
		first->right = merge(first->right, second);
		relax(first);
		return first;
	}
	else
	{
		second->left = merge(first, second->left);
		relax(second);
		return second;
	}
}

pair<Node*, Node*> split(Node* root, int size)
{
	if (root == nullptr) return make_pair(nullptr, nullptr);

	else if (size <= get_size(root->left))
	{
		auto p = split(root->left, size);
		root->left = p.second;
		relax(root);
		return make_pair(p.first, root);
	}
	else
	{
		auto p = split(root->right, size - get_size(root->left) - 1);
		root->right = p.first;
		relax(root);
		return make_pair(root, p.second);
	}
}

Node* insert(Node* tree, int where, int value)
{
	auto p = split(tree, where);
	return merge(merge(p.first, new Node(value)), p.second);
}

Node* append(Node* tree, int value)
{
	return merge(tree, new Node(value));
}

ostream& operator<<(ostream& out, Node* tree)
{
	if (tree == nullptr) return out;
	out << tree->left << tree->value << " " << tree->right;
	return out;
}

int main()
{
	srand(time(0));

	int solider_count = 0;
	cin >> solider_count;

	int order_count = 0;
	cin >> order_count;

	Node* tree = nullptr;
	for (int i = 0; i < solider_count; ++i) 
		tree = append(tree, i + 1);

	for (int i = 0; i < order_count; ++i)
	{
		int l = 0;
		int r = 0;
		cin >> l >> r;

		auto p1 = split(tree, r);
		auto p2 = split(p1.first, l - 1);

		tree = merge(p2.second, merge(p2.first, p1.second));
	}

	cout << tree;

	if (tree != nullptr) delete tree;
	return 0;
} 
