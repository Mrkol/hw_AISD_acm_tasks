#include <iostream>
#include <utility>
#include <cstdlib>
#include <ctime>

using namespace std;


struct Node
{
	int key;
	int priority;
	int size;

	Node* left;
	Node* right;

	Node(int key)
		: key(key), priority(rand()), size(1), left(nullptr), right(nullptr)
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

pair<Node*, Node*> split(Node* root, int key)
{
	if (root == nullptr) return make_pair(nullptr, nullptr);

	if (key < root->key)
	{
		auto p = split(root->left, key);
		root->left = p.second;
		relax(root);
		return make_pair(p.first, root);
	}
	else
	{
		auto p = split(root->right, key);
		root->right = p.first;
		relax(root);
		return make_pair(root, p.second);
	}
}

int kth_statistic(Node* root, int k)
{
	if (root == nullptr) throw -1;

	if (get_size(root->left) == k)
	{
		return root->key;
	}
	else if(k < get_size(root->left))
	{
		return kth_statistic(root->left, k);
	}
	else
	{
		return kth_statistic(root->right, k - get_size(root->left) - 1);
	}
}

Node* insert(Node* tree, int key)
{
	auto p = split(tree, key);
	return merge(merge(p.first, new Node(key)), p.second);
}

Node* remove(Node* tree, int key)
{
	auto p1 = split(tree, key);
	auto p2 = split(p1.first, key - 1);
	delete p2.second;
	return merge(p2.first, p1.second);
}

ostream& operator<<(ostream& out, Node* tree)
{
	if (tree == nullptr) return out;
	out << tree->left << " " << tree->key << " " << tree->right;
	return out;
}

int main()
{
	srand(time(0));
	int opcount = 0;
	cin >> opcount;
	Node* tree = nullptr;
	for (int i = 0; i < opcount; ++i)
	{
		int op = 0;
		cin >> op;
		int val = 0;
		cin >> val;
		if (op == 0)
		{
			cout << kth_statistic(tree, get_size(tree) - val) << endl;
		}
		else if (op == 1)
		{
			tree = insert(tree, val);
		}
		else if (op == -1)
		{
			tree = remove(tree, val);
		}
		//cout << tree << endl;
	}
	if (tree != nullptr) delete tree;
	return 0;
} 
