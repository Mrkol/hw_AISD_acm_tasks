#include <iostream>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

using Integer = long long;

struct Node;

struct Node
{
private:
	static Node empty_;

public:
	Integer size;

	Integer priority;
	Integer seg_size;
	Integer bit_size;

	Node* left;
	Node* right;

	Integer largest_white_offset;
	Integer largest_white_size;

	bool white;

	static Node* empty;

	Node()
		: size(0), priority(0), seg_size(0), bit_size(0),
		left(this), right(this),
		largest_white_offset(0), largest_white_size(0), white(false)
	{

	}

	Node(Integer size, bool white)
		: size(size), priority(rand()), seg_size(1), 
		bit_size(size), left(empty), right(empty),
		largest_white_offset(0), 
		largest_white_size(size * white), white(white)
	{

	}

	~Node()
	{
		if (left != Node::empty) delete left;
		if (right != Node::empty) delete right;
	}
};

Node Node::empty_ = Node();
Node* Node::empty = &Node::empty_;

void relax(Node* root)
{
	if (root == Node::empty) return;
	root->seg_size = root->left->seg_size + root->right->seg_size + 1;
	root->bit_size = root->left->bit_size + root->right->bit_size + root->size;

	if (root->white 
		&& root->size > root->left->largest_white_size
		&& root->size >= root->right->largest_white_size)
	{
		root->largest_white_size = root->size;
		root->largest_white_offset = root->left->bit_size;
	}
	else if (
		root->left->largest_white_size >= root->right->largest_white_size)
	{
		root->largest_white_size = root->left->largest_white_size;
		root->largest_white_offset = root->left->largest_white_offset;
	}
	else
	{
		root->largest_white_size = root->right->largest_white_size;
		root->largest_white_offset = 
			root->left->bit_size + root->size + root->right->largest_white_offset;		
	}
}

Node* merge(Node* first, Node* second)
{
	if (first == Node::empty) return second;
	if (second == Node::empty) return first;

	if (first->priority > second->priority)
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

pair<Node*, Node*> split_seg(Node* root, Integer size)
{
	if (root == Node::empty) return make_pair(Node::empty, Node::empty);

	else if (size <= root->left->seg_size)
	{
		auto p = split_seg(root->left, size);
		root->left = p.second;
		relax(root);
		return make_pair(p.first, root);
	}
	else
	{
		auto p = split_seg(root->right, size - root->left->seg_size - 1);
		root->right = p.first;
		relax(root);
		return make_pair(root, p.second);
	}
}

pair<Node*, Node*> split_bit(Node* root, Integer size)
{
	if (root == Node::empty) return make_pair(Node::empty, Node::empty);

	else if (size <= root->left->bit_size)
	{
		auto p = split_bit(root->left, size);
		root->left = p.second;
		relax(root);
		return make_pair(p.first, root);
	}
	else
	{
		auto p = split_bit(root->right, size - root->left->bit_size - root->size);
		root->right = p.first;
		relax(root);
		return make_pair(root, p.second);
	}
}

Node* insert(Node* tree, Integer size)
{
	if (tree == Node::empty) return tree;
	if (size > tree->largest_white_size) return tree;

	auto p1 = split_bit(tree, tree->largest_white_offset);
	auto p2 = split_seg(p1.second, 1);

	Node* left = p1.first;
	Node* target = p2.first;
	Node* right = p2.second;

	if (size < target->size)
	{
		Node* black = new Node(size, false);
		Node* white = new Node(p2.first->size - size, true);
		delete target;
		target = merge(black, white);
	}
	else
	{
		//So that the chance of forgetting to update any of the fields is precisely zero
		Node* black = new Node(target->size, false);
		delete target;
		target = black;
	}
	return merge(left, merge(target, right));
}

Node* remove(Node* tree, Integer offset)
{
	if (tree == Node::empty) return tree;
	if (offset >= tree->bit_size) return tree;

	auto p1 = split_bit(tree, offset);
	auto p2 = split_seg(p1.second, 1); //nice
	auto p3 = split_seg(p1.first, p1.first->seg_size - 1);
	auto p4 = split_seg(p2.second, 1);

	Node* leftmost = p3.first;
	Node* left = p3.second;
	Node* target = p2.first;
	Node* right = p4.first;
	Node* rightmost = p4.second;

	Node* center = Node::empty;
	if (left->white && right->white)
	{
		center = new Node(left->size + target->size + right->size, true);
		delete left;
		delete target;
		delete right;
	}
	else if (left->white)
	{
		center = new Node(left->size + target->size, true);
		delete left;
		delete target;
		center = merge(center, right);
	}
	else if (right->white)
	{
		center = new Node(target->size + right->size, true);
		delete right;
		delete target;
		center = merge(left, center);
	}
	else
	{
		center = new Node(target->size, true);
		delete target;
		center = merge(left, merge(center, right));
	}

	return merge(leftmost, merge(center, rightmost));
}

ostream& operator<<(ostream& out, Node* tree)
{
	if (tree == Node::empty) return out;
	out << tree->left;
	for (int i = 0; i < tree->size; ++i)
		cout << (tree->white ? '.' : '#');
	out << tree->right;
	return out;
}

int main()
{
	Integer memory = 0;
	int requests = 0;
	cin >> memory >> requests;

	Node* tree = new Node(memory, true);

	vector<int> requestOffsets(requests, -1);

	//cout << tree << endl;
	for (int i = 0; i < requests; ++i)
	{
		Integer val = 0;
		cin >> val;

		if (val > 0)
		{
			if (val <= tree->largest_white_size)
			{
				requestOffsets[i] = tree->largest_white_offset;
				cout << (1 + tree->largest_white_offset) << endl;
				tree = insert(tree, val);
			}
			else
			{
				cout << -1 << endl;
			}
		}
		else
		{
			val = -val;
			if (requestOffsets[val - 1] != -1)
			{
				tree = remove(tree, requestOffsets[val - 1]);
			}
		}

		//cout << tree << endl;
	}

	if (tree != Node::empty) delete tree;
	return 0;
} 
