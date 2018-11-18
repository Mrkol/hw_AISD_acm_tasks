#include <iostream>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <functional>


using Integer = long long;

class CTNode
{
	static CTNode _Null;

	CTNode* _left;
	CTNode* _right;
	
	Integer _value;
	Integer _priority;

	Integer _size;
	Integer _sum;
	Integer _first;
	Integer _last;
	Integer _increasingPrefix;
	Integer _decreasingPrefix;
	Integer _increasingPostfix;
	Integer _decreasingPostfix;

	Integer _promisedIncrement;
	Integer _promisedValue;
	bool _promisedSet;
	bool _promisedReverse;

	void _relax();
	void _push();

	int _lowerBound(Integer, bool);

	friend std::ostream& operator<<(std::ostream&, CTNode*);
	friend void dump(std::ostream& out, CTNode* node);

public:
	CTNode();

	CTNode(Integer);

	void IncreaseAll(Integer);
	void SetAll(Integer);
	void Reverse();

	Integer GetSum();

	~CTNode();

	static CTNode* Null;
	static CTNode* Merge(CTNode*, CTNode*);
	static std::pair<CTNode*, CTNode*> Split(CTNode*, int);
	static CTNode* Insert(CTNode*, int, Integer);
	static CTNode* Remove(CTNode*, int);
	static CTNode* NextPermutation(CTNode*);
	static CTNode* PreviousPermutation(CTNode*);
};

CTNode CTNode::_Null = CTNode();
CTNode* CTNode::Null = &CTNode::_Null;

CTNode::CTNode()
	: _left(Null), _right(Null), _value(0), _priority(0), _size(0),
	_sum(0), _first(0), _last(0),
	_increasingPrefix(0), _decreasingPrefix(0),
	_increasingPostfix(0), _decreasingPostfix(0),
	_promisedIncrement(0), _promisedValue(0), _promisedSet(false), _promisedReverse(false) 
{

}

CTNode::CTNode(Integer value)
	: _left(Null), _right(Null), _value(value), _priority(std::rand()), _size(1),
	_sum(value), _first(value), _last(value),
	_increasingPrefix(1), _decreasingPrefix(1),
	_increasingPostfix(1), _decreasingPostfix(1),
	_promisedIncrement(0), _promisedValue(0), _promisedSet(false), _promisedReverse(false)
{

}

CTNode::~CTNode()
{
	if (_left != Null) delete _left;
	if (_right != Null) delete _right;
}

void CTNode::IncreaseAll(Integer increment)
{
	if (this == Null) return;

	_promisedIncrement += increment;
	_sum += increment * _size;
	_value += increment;
	_first += increment;
	_last += increment;
}

void CTNode::SetAll(Integer value)
{
	if (this == Null) return;

	_promisedIncrement = 0;
	_promisedSet = true;
	_promisedValue = value;
	_sum = value * _size;
	_value = value;
	_increasingPrefix = _size;
	_decreasingPrefix = _size;
	_increasingPostfix = _size;
	_decreasingPostfix = _size;
	_first = value;
	_last = value;
}

void CTNode::Reverse()
{
	if (this == Null) return;

	std::swap(_left, _right);
	_promisedReverse = !_promisedReverse;
	std::swap(_increasingPrefix, _increasingPostfix);
	std::swap(_decreasingPrefix, _decreasingPostfix);
	std::swap(_first, _last);
}

CTNode* CTNode::NextPermutation(CTNode* root)
{
	if (root->_increasingPostfix == root->_size)
	{
		root->Reverse();
		return root;
	}

	auto p = Split(root, -root->_increasingPostfix);
	int swap_pos = p.second->_lowerBound(p.first->_last, true) - 1;

	auto p1 = Split(p.first, -1);
	auto p2 = Split(p.second, swap_pos);
	auto p3 = Split(p2.second, 1);
	CTNode* left = Merge(p1.first, p3.first);
	CTNode* right = Merge(Merge(p2.first, p1.second), p3.second);
	right->Reverse();
	return Merge(left, right);
}

CTNode* CTNode::PreviousPermutation(CTNode* root)
{
	if (root->_decreasingPostfix == root->_size)
	{
		root->Reverse();
		return root;
	}

	auto p = Split(root, -root->_decreasingPostfix);
	int swap_pos = p.second->_lowerBound(p.first->_last, false) - 1;

	auto p1 = Split(p.first, -1);
	auto p2 = Split(p.second, swap_pos);
	auto p3 = Split(p2.second, 1);

	CTNode* left = Merge(p1.first, p3.first);
	CTNode* right = Merge(Merge(p2.first, p1.second), p3.second);
	right->Reverse();
	return Merge(left, right);
}

Integer CTNode::GetSum()
{
	return _sum;
}

int CTNode::_lowerBound(Integer value, bool reversed)
{
	if (this == Null) return 0;

	_push();

	if (!reversed ? (value <= _value) : (_value <= value))
	{
		return _left->_lowerBound(value, reversed);
	}
	else
	{
		return _left->_size + 1 + _right->_lowerBound(value, reversed);
	}
}

void CTNode::_relax()
{
	if (this == Null) return;

	_size = _left->_size + _right->_size + 1;
	_sum = _left->_sum + _right->_sum + _value;

	if (_left != Null) _first = _left->_first;
	else _first = _value;

	if (_right != Null) _last = _right->_last;
	else _last = _value;

	_increasingPrefix = _left->_increasingPrefix;
	if (_left->_increasingPrefix == _left->_size 
		&& (_value >= _left->_last || _left == Null))
	{
		_increasingPrefix += 1;
		if (_right->_first >= _value) _increasingPrefix += _right->_increasingPrefix;
	}
	
	_decreasingPrefix = _left->_decreasingPrefix;
	if (_left->_decreasingPrefix == _left->_size 
		&& (_value <= _left->_last || _left == Null))
	{
		_decreasingPrefix += 1;
		if (_right->_first <= _value) _decreasingPrefix += _right->_decreasingPrefix;
	}

	_increasingPostfix = _right->_increasingPostfix;
	if (_right->_increasingPostfix == _right->_size 
		&& (_value >= _right->_first || _right == Null))
	{
		_increasingPostfix += 1;
		if (_left->_last >= _value) _increasingPostfix += _left->_increasingPostfix;
	}

	_decreasingPostfix = _right->_decreasingPostfix;
	if (_right->_decreasingPostfix == _right->_size 
		&& (_value <= _right->_first || _right == Null))
	{
		_decreasingPostfix += 1;
		if (_left->_last <= _value) _decreasingPostfix += _left->_decreasingPostfix;
	}
}

void CTNode::_push()
{
	if (this == Null) return;

	if (_promisedSet)
	{
		_left->SetAll(_promisedValue);
		_right->SetAll(_promisedValue);
	}
	_left->IncreaseAll(_promisedIncrement);
	_right->IncreaseAll(_promisedIncrement);
	if (_promisedReverse)
	{
		_left->Reverse();
		_right->Reverse();
	}
	_promisedReverse = false;
	_promisedSet = false;
	_promisedIncrement = 0;
}

CTNode* CTNode::Merge(CTNode* first, CTNode* second)
{
	if (first == Null) return second;
	if (second == Null) return first;

	if (first->_priority > second->_priority)
	{
		first->_push();
		first->_right = Merge(first->_right, second);
		first->_relax();
		return first;
	}
	else
	{
		second->_push();
		second->_left = Merge(first, second->_left);
		second->_relax();
		return second;
	}
}

std::pair<CTNode*, CTNode*> CTNode::Split(CTNode* root, int size)
{
	if (root == Null) return std::make_pair(Null, Null);
	if (size < 0) size += root->_size;

	if (size <= root->_left->_size)
	{
		root->_push();
		auto p = Split(root->_left, size);
		root->_left = p.second;
		root->_relax();
		return std::make_pair(p.first, root);
	}
	else
	{
		root->_push();
		auto p = Split(root->_right, size - root->_left->_size - 1);
		root->_right = p.first;
		root->_relax();
		return std::make_pair(root, p.second);
	}
}

CTNode* CTNode::Remove(CTNode* tree, int which)
{
	if (which < 0) which += tree->_size;
	auto p1 = Split(tree, which);
	auto p2 = Split(p1.second, 1);
	if (p2.first != Null) delete p2.first; 
	return Merge(p1.first, p2.second);
}

CTNode* CTNode::Insert(CTNode* tree, int where, Integer value)
{
	if (where < 0) where += tree->_size + 1;
	auto p = Split(tree, where);
	return Merge(Merge(p.first, new CTNode(value)), p.second);
}

std::ostream& operator<<(std::ostream& out, CTNode* tree)
{
	if (tree == CTNode::Null) return out;
	tree->_push();
	out << tree->_left << tree->_value << " " << tree->_right;
	return out;
}

int main()
{
	std::srand(0);

	int count = 0;
	std::cin >> count;

	CTNode* tree = CTNode::Null;
	for (int i = 0; i < count; ++i)
	{
		int value = 0;
		std::cin >> value;
		tree = CTNode::Insert(tree, -1, value);
	}

	int request_count = 0;
	std::cin >> request_count;

	for (int q = 0; q < request_count; ++q)
	{
		int request = 0;
		std::cin >> request;
		switch (request)
		{
			case 1: //segment sum
				{
					int l = 0;
					int r = 0;
					std::cin >> l >> r;
					auto p1 = CTNode::Split(tree, r + 1);
					auto p2 = CTNode::Split(p1.first, l);
					std::cout << p2.second->GetSum() << std::endl;
					tree = CTNode::Merge(CTNode::Merge(p2.first, p2.second), p1.second);
				}
				break;

			case 2: //insert at
				{
					Integer value = 0;
					int pos = 0;
					std::cin >> value >> pos;
					tree = CTNode::Insert(tree, pos, value);
				}
				break;

			case 3: //remove at
				{
					int pos = 0;
					std::cin >> pos;
					tree = CTNode::Remove(tree, pos);
				}
				break;

			case 4: //set on segment
				{
					Integer value = 0;
					int l = 0;
					int r = 0;
					std::cin >> value >> l >> r;
					auto p1 = CTNode::Split(tree, r + 1);
					auto p2 = CTNode::Split(p1.first, l);
					p2.second->SetAll(value);
					tree = CTNode::Merge(CTNode::Merge(p2.first, p2.second), p1.second);
				}
				break;

			case 5: //add on segment
				{
					Integer value = 0;
					int l = 0;
					int r = 0;
					std::cin >> value >> l >> r;
					auto p1 = CTNode::Split(tree, r + 1);
					auto p2 = CTNode::Split(p1.first, l);
					p2.second->IncreaseAll(value);
					tree = CTNode::Merge(CTNode::Merge(p2.first, p2.second), p1.second);
				}
				break;

			case 6: //next perm
				{
					int l = 0;
					int r = 0;
					std::cin >> l >> r;
					auto p1 = CTNode::Split(tree, r + 1);
					auto p2 = CTNode::Split(p1.first, l);
					p2.second = CTNode::NextPermutation(p2.second);
					tree = CTNode::Merge(CTNode::Merge(p2.first, p2.second), p1.second);
				}
				break;

			case 7: //prev perm
				{
					int l = 0;
					int r = 0;
					std::cin >> l >> r;
					auto p1 = CTNode::Split(tree, r + 1);
					auto p2 = CTNode::Split(p1.first, l);
					p2.second = CTNode::PreviousPermutation(p2.second);
					tree = CTNode::Merge(CTNode::Merge(p2.first, p2.second), p1.second);
				}
				break;
		}
	}
	std::cout << tree << std::endl;

	if (tree != CTNode::Null) delete tree;
	return 0;
} 
