#include <iostream>
#include <utility>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;


struct Node
{
	int value;
	int priority;
	int size;

	Node* left;
	Node* right;

	Node* parent;

	bool reverse;

	bool cycle;

	Node()
		: value(0), priority(0), size(0),
		left(null), right(null), parent(null),
		reverse(false), cycle(false)
	{

	}

	Node(int value)
		: value(value), priority(rand()), size(1),
		left(null), right(null), parent(null),
		reverse(false), cycle(false)
	{

	}

	~Node()
	{

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

void Reverse(Node* root)
{
	if (root == Node::null) return;

	root->reverse = !root->reverse;

	swap(root->left, root->right);
}

void push(Node* root)
{
	if (root == Node::null) return;

	if (root->reverse)
	{
		Reverse(root->left);
		Reverse(root->right);
		root->reverse = false;
	}
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

Node* getRoot(Node* tree)
{
	if (tree->parent == Node::null) return tree;

	return getRoot(tree->parent);
}

int getIndex(Node* tree)
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

void AddRoad(vector<Node*>& nodes, int first, int second)
{
	Node* root1 = getRoot(nodes[first]);
	Node* root2 = getRoot(nodes[second]);

	if (root1 == root2)
	{
		root1->cycle = true;
		return;
	}

	int ind1 = getIndex(nodes[first]);
	int ind2 = getIndex(nodes[second]);

	if ((ind1 != 0 && ind1 != root1->size - 1) 
		|| (ind2 != 0 && ind2 != root2->size - 1))
	{
		cerr << "EVERYTHING BROKE" << endl;
		throw -1;
	}

	if (ind1 == 0) Reverse(root1);
	if (ind2 != 0) Reverse(root2);
	merge(root1, root2);
}

void CutRoad(vector<Node*>& nodes, int first, int second)
{
	Node* root1 = getRoot(nodes[first]);
	Node* root2 = getRoot(nodes[second]);

	if (root1 != root2)
	{
		cerr << "HOLY SHIT" << endl;
		throw -1;
	}

	int ind1 = getIndex(nodes[first]);
	int ind2 = getIndex(nodes[second]);

	if ((ind1 == 0 && ind2 == root1->size - 1)
		|| (ind2 == 0 && ind1 == root1->size - 1))
	{
		if (root1->cycle)
		{
			root1->cycle = false;
			return;
		}
		else if (root1->size > 2)
		{
			throw -1;
		}
	}

	if (abs(ind1 - ind2) != 1) throw -1;

	bool mergeback = root1->cycle;
	root1->cycle = false;

	auto p = split(root1, max(ind1, ind2));

	if (mergeback)
	{
		merge(p.second, p.first);
	}
}

int getDistance(vector<Node*>& nodes, int first, int second)
{
	Node* root1 = getRoot(nodes[first]);
	Node* root2 = getRoot(nodes[second]);

	if (root1 != root2)
	{
		return -1;
	}

	int ind1 = getIndex(nodes[first]);
	int ind2 = getIndex(nodes[second]);

	if (ind1 == ind2) return 0;

	if (root1->cycle)
		return min(abs(ind1 - ind2), root1->size - abs(ind1 - ind2)) - 1;

	return abs(ind1 - ind2) - 1;
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

	int cities = 0;
	cin >> cities;

	int roads = 0;
	cin >> roads;

	int requests = 0;
	cin >> requests;

	vector<Node*> nodes(cities);
	for (int i = 0; i < cities; ++i)
		nodes[i] = new Node(i + 1);

	for (int i = 0; i < roads; ++i)
	{
		int first = 0;
		int second = 0;
		cin >> first >> second;
		--first;
		--second;

		AddRoad(nodes, first, second);
	}


	for (int r = 0; r < requests; ++r)
	{
		char c = 0;
		cin >> c;
		int first = 0;
		int second = 0;
		cin >> first >> second;
		--first;
		--second;
		if (c == '-')
		{
			CutRoad(nodes, first, second);
		}
		else if (c == '+')
		{
			AddRoad(nodes, first, second);
		}
		else if (c == '?')
		{
			int dist = getDistance(nodes, first, second);
			cout << dist << endl;
		}
		else
		{
			cerr << "invalid char: " << c << endl; 
			throw -1;
		}

		/*cout << "========" << endl;
		for (int i = 0; i < cities; ++i)
		{
			cout << getRoot(nodes[i]) << endl;
		}
		cout << "========" << endl;*/
	}


	for (int i = 0; i < cities; ++i)
		delete nodes[i];
	
	return 0;
} 
