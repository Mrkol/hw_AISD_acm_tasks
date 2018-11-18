#include <iostream>
#include <utility>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <tuple>
#include <map>

using namespace std;


struct Node
{
	int type;
	int length;
	int nextoffset;
	int priority;
	int size;

	Node* left;
	Node* right;

	Node* parent;

	Node()
		: type(-1), length(0), nextoffset(0), priority(0), size(0),
		left(null), right(null), parent(null)
	{

	}

	Node(int type, int length)
		: type(type), length(length), nextoffset(0), priority(rand()), size(1),
		left(null), right(null), parent(null)
	{

	}

	~Node()
	{
		if (left != null) delete left;
		if (right != null) delete right;
	}

	static Node _null;
	static Node* null;
};

Node Node::_null = Node();
Node* Node::null = &Node::_null;

void setParent(Node* child, Node* parent)
{
	if (child == Node::null) return;
	child->parent = parent;
}

void relax(Node* root)
{
	if (root == Node::null) return;
	root->size = root->left->size + root->right->size + 1;
}

void push(Node* root)
{
	if (root == Node::null) return;


}

Node* merge(Node* first, Node* second)
{
	if (first == Node::null) return second;
	if (second == Node::null) return first;

	if (first->priority > second->priority)
	{
		push(first);
		first->right = merge(first->right, second);
		relax(first);

		setParent(first->right, first);
		setParent(first, Node::null);
		return first;
	}
	else
	{
		push(second);
		second->left = merge(first, second->left);
		relax(second);

		setParent(second->left, second);
		setParent(second, Node::null);
		return second;
	}
}

pair<Node*, Node*> split(Node* root, int size)
{
	if (root == Node::null) return make_pair(Node::null, Node::null);

	else if (size <= root->left->size)
	{
		push(root);
		auto p = split(root->left, size);
		root->left = p.second;
		relax(root);
		
		setParent(root->left, root);
		setParent(p.first, Node::null);
		setParent(root, Node::null);
		return make_pair(p.first, root);
	}
	else
	{
		push(root);
		auto p = split(root->right, size - root->left->size - 1);
		root->right = p.first;
		relax(root);

		setParent(root->right, root);
		setParent(root, Node::null);
		setParent(p.second, Node::null);
		return make_pair(root, p.second);
	}
}

int index(Node* tree)
{
	if (tree->parent == Node::null) 
	{
		push(tree);
		return tree->left->size;
	}

	int pindex = getIndex(tree->parent);
	push(tree);

	if (tree == tree->parent->left)
		pindex -= tree->right->size + 1;
	else
		pindex += tree->left->size + 1;

	return pindex;
}

Node* at(Node* tree, int pos)
{
	if (tree == Node::null) return Node::null;

	if (pos == tree->left->size)
	{
		return tree;
	}
	else if (pos < tree->left->size)
	{
		return at(tree->left, pos);
	}
	else
	{
		return at(tree->right, pos - tree->left->size - 1);
	}
}

using Box = tuple<Node*, map<int, Node*>, map<int, int>>;

Node*& tree(Box& box)
{
	return get<0>(box);
}

map<int, Node*>& firsts(Box& box)
{
	return get<1>(box);
}

map<int, int>& counts(Box& box)
{
	return get<2>(box);
}

void insert(Box& box, int type, int length)
{
	Node* newnode = new Node(type, length);
	newnode->nextoffset = index(poses(box)[type]) + 1;

	poses(box)[type] = newnode;
	counts(box)[type] += length;
	tree(box) = merge(newnode, tree(box));
}

int remove(Box& box, int type, int depth)
{
	stack<Node*> separated;
	separated.push(tree(box));


}

std::ostream& operator<<(std::ostream& out, Node* tree)
{
	if (tree == Node::null) return out;
	push(tree);
	out << tree->left << tree->value << " " << tree->right;
	return out;
}

int main()
{
	srand(time(0));

	int obj_count = 0;
	cin >> obj_count;

	int type_count = 0;
	cin >> type_count;

	int box_count = 0;
	cin >> box_count;

	vector<Box> boxes(box_count);
	for (int i = 0; i < box_count; ++i)
	{
		int box_size = 0;
		cin >> box_size;

		get<0>(boxes[i]) = Node::null;
		for (int j = 0; j< box_size; ++j)
		{

		}
	}
	
	return 0;
} 
