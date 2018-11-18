#include <functional>
#include <iostream>
#include <utility>
#include <climits>
#include <random>


class CTNode
{
public:
	using ValueType = long long;
	using Modifier = std::function<CTNode*(CTNode*)>;

private:
	static CTNode _null;
	static CTNode* _permute(CTNode*, bool);
	static std::default_random_engine _engine;
	static std::uniform_int_distribution<long long> _priorityDistribution;
	static long long _generatePriority();

	CTNode* _left;
	CTNode* _right;
	
	ValueType _value;
	long long _priority;

	std::size_t _size;
	ValueType _sum;
	ValueType _first;
	ValueType _last;
	std::size_t _increasingPrefix;
	std::size_t _decreasingPrefix;
	std::size_t _increasingPostfix;
	std::size_t _decreasingPostfix;

	ValueType _promisedIncrement;
	ValueType _promisedValue;
	bool _promisedSet;
	bool _promisedReverse;

	void _relax();
	void _push();

	void _toVector(std::vector<ValueType>&);

	std::size_t _lowerBound(ValueType, bool);

	friend class IntegerString;
public:	
	CTNode();

	CTNode(const CTNode&);

	explicit CTNode(ValueType);

	operator std::vector<ValueType>();

	void IncreaseAll(ValueType);
	void SetAll(ValueType);
	void Reverse();

	ValueType GetSum() const;

	~CTNode();

	static CTNode* Null;
	static CTNode* Merge(CTNode*, CTNode*);
	static std::pair<CTNode*, CTNode*> Split(CTNode*, std::size_t);
	static CTNode* Insert(CTNode*, std::size_t, ValueType);
	static CTNode* PushFront(CTNode*, ValueType);
	static CTNode* PushBack(CTNode*, ValueType);
	static CTNode* Remove(CTNode*, std::size_t);
	static CTNode* NextPermutation(CTNode*);
	static CTNode* PreviousPermutation(CTNode*);
	static CTNode* ModifySubsegment(CTNode*, std::size_t, std::size_t, 
		std::function<CTNode*(CTNode*)>);
	static Modifier MakeAdder(ValueType);
	static Modifier MakeSetter(ValueType);
	static Modifier MakeReverser();
	static Modifier MakePermuter(int);
};

CTNode CTNode::_null;
std::uniform_int_distribution<long long> 
	CTNode::_priorityDistribution(-LLONG_MAX, LLONG_MAX);
std::default_random_engine CTNode::_engine;
CTNode* CTNode::Null = &CTNode::_null;

CTNode::CTNode()
	: _left(Null), _right(Null), _value(0), _priority(0), _size(0),
	_sum(0), _first(0), _last(0),
	_increasingPrefix(0), _decreasingPrefix(0),
	_increasingPostfix(0), _decreasingPostfix(0),
	_promisedIncrement(0), _promisedValue(0), _promisedSet(false), _promisedReverse(false) 
{

}

CTNode::CTNode(ValueType value)
	: _left(Null), _right(Null), _value(value), _priority(std::rand()), _size(1),
	_sum(value), _first(value), _last(value),
	_increasingPrefix(1), _decreasingPrefix(1),
	_increasingPostfix(1), _decreasingPostfix(1),
	_promisedIncrement(0), _promisedValue(0), _promisedSet(false), _promisedReverse(false)
{

}

CTNode::CTNode(const CTNode& other)
	: _left(Null), _right(Null), _value(other._value), _priority(other._priority), 
		_size(other._size), _sum(other._sum), _first(other._first), _last(other._last),
		_increasingPrefix(other._increasingPrefix), 
		_decreasingPrefix(other._decreasingPrefix),
		_increasingPostfix(other._increasingPostfix), 
		_decreasingPostfix(other._decreasingPostfix),
		_promisedIncrement(other._promisedIncrement), 
		_promisedValue(other._promisedValue), 
		_promisedSet(other._promisedSet), 
		_promisedReverse(other._promisedReverse)
{
	if (other._left != Null)
		_left = new CTNode(*other._left);
	if (other._right != Null)
		_right = new CTNode(*other._right);
}

CTNode::~CTNode()
{
	if (_left != Null) delete _left;
	if (_right != Null) delete _right;
}

void CTNode::_toVector(std::vector<ValueType>& result)
{
	if (this == Null) return;
	_push();
	_left->_toVector(result);
	result.push_back(_value);
	_right->_toVector(result);
}

CTNode::operator std::vector<ValueType>()
{
	std::vector<ValueType> result;
	result.reserve(_size);
	_toVector(result);
	return result;
}

void CTNode::IncreaseAll(ValueType increment)
{
	if (this == Null) return;

	_promisedIncrement += increment;
	_sum += increment * _size;
	_value += increment;
	_first += increment;
	_last += increment;
}

void CTNode::SetAll(ValueType value)
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

long long CTNode::_generatePriority()
{
	return _priorityDistribution(_engine);
}

CTNode* CTNode::_permute(CTNode* root, bool prev)
{
	std::size_t seq_len = prev ? root->_decreasingPostfix : root->_increasingPostfix;
	if (seq_len == root->_size)
	{
		root->Reverse();
		return root;
	}

	auto p = Split(root, root->_size - seq_len);
	std::size_t swap_pos = p.second->_lowerBound(p.first->_last, !prev) - 1;

	auto p1 = Split(p.first, p.first->_size - 1);
	auto p2 = Split(p.second, swap_pos);
	auto p3 = Split(p2.second, 1);

	CTNode* left = Merge(p1.first, p3.first);
	CTNode* right = Merge(Merge(p2.first, p1.second), p3.second);
	right->Reverse();
	return Merge(left, right);
}

CTNode* CTNode::NextPermutation(CTNode* root)
{
	return _permute(root, false);
}

CTNode* CTNode::PreviousPermutation(CTNode* root)
{
	return _permute(root, true);
}

CTNode::ValueType CTNode::GetSum() const
{
	return _sum;
}

std::size_t CTNode::_lowerBound(ValueType value, bool reversed)
{
	if (this == Null) return 0;

	_push();

	if (!reversed ? (value <= _value) : (_value <= value))
		return _left->_lowerBound(value, reversed);
	else
		return _left->_size + 1 + _right->_lowerBound(value, reversed);
}

void CTNode::_relax()
{
	if (this == Null) return;

	_size = _left->_size + _right->_size + 1;
	_sum = _left->_sum + _right->_sum + _value;

	_first = _left != Null ? _left->_first : _value;
	_last = _right != Null ? _right->_last : _value;

	auto sequenceRelax = [this](bool increasing, bool prefix)
		{
			auto getSequence = [increasing, prefix](CTNode* root) -> std::size_t&
				{
					return increasing ?
						(prefix ? root->_increasingPrefix : root->_increasingPostfix) 
						: (prefix ? root->_decreasingPrefix : root->_decreasingPostfix);
				};

			auto compare = [increasing](ValueType a, ValueType b) -> bool 
				{
					return increasing ? a >= b : a <= b;
				};

			auto firstChild = prefix ? _left : _right;
			auto secondChild = prefix ? _right : _left;

			ValueType firstEnd = prefix ? _left->_last : _right->_first;
			ValueType secondEnd = prefix ? _right->_first : _left->_last;

			getSequence(this) = getSequence(firstChild);
			if (getSequence(firstChild) == firstChild->_size 
				&& (compare(_value, firstEnd) || firstChild == Null))
			{
				getSequence(this) += 1;
				if (compare(secondEnd, _value)) 
					getSequence(this) += getSequence(secondChild);
			}
		};

	sequenceRelax(true, true);
	sequenceRelax(false, true);
	sequenceRelax(true, false);
	sequenceRelax(false, false);
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

std::pair<CTNode*, CTNode*> CTNode::Split(CTNode* root, std::size_t size)
{
	if (root == Null) return std::make_pair(Null, Null);

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

CTNode* CTNode::Remove(CTNode* tree, std::size_t which)
{
	return ModifySubsegment(tree, which, which, 
		[](CTNode* root)
		{
			if (root != Null) 
				delete root;
			return Null;
		});
}

CTNode* CTNode::Insert(CTNode* tree, std::size_t where, ValueType value)
{
	auto p = Split(tree, where);
	return Merge(Merge(p.first, new CTNode(value)), p.second);
}

CTNode* CTNode::PushFront(CTNode* root, ValueType value)
{
	return Insert(root, 0, value);
}

CTNode* CTNode::PushBack(CTNode* root, ValueType value)
{
	return Insert(root, root->_size, value);
}

CTNode* CTNode::ModifySubsegment(CTNode* root, std::size_t left, std::size_t right, 
	Modifier modifier)
{
	auto p1 = CTNode::Split(root, right + 1);
	auto p2 = CTNode::Split(p1.first, left);
	return CTNode::Merge(CTNode::Merge(p2.first, modifier(p2.second)), p1.second);
}

CTNode::Modifier CTNode::MakeAdder(ValueType value)
{
	return [value](CTNode* root) -> CTNode*
		{
			root->IncreaseAll(value);
			return root;
		};
}

CTNode::Modifier CTNode::MakeSetter(ValueType value)
{
	return [value](CTNode* root) -> CTNode*
		{
			root->SetAll(value);
			return root;
		};
}

CTNode::Modifier CTNode::MakeReverser()
{
	return [](CTNode* root) -> CTNode*
		{
			root->Reverse();
			return root;
		};
}

CTNode::Modifier CTNode::MakePermuter(int delta)
{
	return [delta](CTNode* root) -> CTNode*
		{
			int d = delta;
			for (; d > 0; --d)
				root = NextPermutation(root);
			for (; d < 0; ++d)
				root = PreviousPermutation(root);
			return root;
		};
}










class IntegerString
{
	CTNode* _root;

public:
	using ValueType = long long;

	IntegerString();

	IntegerString(const IntegerString&);

	operator std::vector<ValueType>();

	std::size_t GetSize();

	void Insert(std::size_t, ValueType);
	void Remove(std::size_t);
	void PushBack(ValueType);
	void IncreaseOnSubsegment(std::size_t, std::size_t, ValueType);
	void SetOnSubsegment(std::size_t, std::size_t, ValueType);
	void NextPermutationOnSubsegment(std::size_t, std::size_t);
	void PreviousPermutationOnSubsegment(std::size_t, std::size_t);
	ValueType GetSumOnSubsegment(std::size_t, std::size_t);

	~IntegerString();
};

IntegerString::IntegerString()
	: _root(CTNode::Null)
{

}

IntegerString::IntegerString(const IntegerString& other)
{
	if (other._root != CTNode::Null)
		_root = new CTNode(*other._root);
}

IntegerString::operator std::vector<ValueType>()
{
	return *_root;
}

std::size_t IntegerString::GetSize()
{
	return _root->_size;
}

void IntegerString::Insert(std::size_t where, ValueType value)
{
	_root = CTNode::Insert(_root, where, value);
}

void IntegerString::Remove(std::size_t which)
{
	_root = CTNode::Remove(_root, which);
}

void IntegerString::PushBack(ValueType value)
{
	_root = CTNode::PushBack(_root, value);
}

void IntegerString::IncreaseOnSubsegment(
	std::size_t left, std::size_t right, ValueType value)
{
	_root = CTNode::ModifySubsegment(_root, left, right, CTNode::MakeAdder(value));
}

void IntegerString::SetOnSubsegment(
	std::size_t left, std::size_t right, ValueType value)
{
	_root = CTNode::ModifySubsegment(_root, left, right, CTNode::MakeSetter(value));
}

void IntegerString::NextPermutationOnSubsegment(
	std::size_t left, std::size_t right)
{
	_root = CTNode::ModifySubsegment(_root, left, right, CTNode::MakePermuter(1));
}

void IntegerString::PreviousPermutationOnSubsegment(
	std::size_t left, std::size_t right)
{
	_root = CTNode::ModifySubsegment(_root, left, right, CTNode::MakePermuter(-1));
}

IntegerString::ValueType IntegerString::GetSumOnSubsegment(
	std::size_t left, std::size_t right)
{
	ValueType result;

	_root = CTNode::ModifySubsegment(_root, 
		left, right, 
		[&result](CTNode* root) 
		{
			result = root->GetSum();
			return root;
		});

	return result;
}

IntegerString::~IntegerString()
{
	if (_root != CTNode::Null) delete _root;
}




enum RequestType
{
	Sum = 1,
	Insert = 2,
	Remove = 3,
	Set = 4,
	Add = 5,
	NextPermutation = 6,
	PreviousPermutation = 7
};

struct Request
{
	RequestType Type;
	std::size_t FirstPosition;
	std::size_t SecondPosition;
	long long Value;

	static Request Read(std::istream&);

private:
	Request() = default;
};

Request Request::Read(std::istream& in)
{
	Request request;
	in >> ((int&) request.Type);

	switch (request.Type)
	{
		case Add:
		case Set:
		case Insert:
			in >> request.Value;
		default:
			break;
	}
	switch (request.Type)
	{
		case Sum:
		case Add:
		case Set:
		case NextPermutation:
		case PreviousPermutation:
			in >> request.FirstPosition >> request.SecondPosition;
			break;

		case Insert:
		case Remove:
			in >> request.FirstPosition;
			break;

		default:
			break;
	}

	return request;
}

struct InputDataset
{
	std::vector<long long> Values;
	std::vector<Request> Requests;
};

struct OutputDataset
{
	std::vector<long long> Sums;
	std::vector<long long> Leftovers;
};

InputDataset ReadData(std::istream& in)
{
	std::size_t count = 0;
	in >> count;

	std::vector<long long> values;
	values.reserve(count);
	for (std::size_t i = 0; i < count; ++i)
	{
		long long value = 0;
		in >> value;
		values.push_back(value);
	}

	std::size_t requestCount = 0;
	in >> requestCount;

	std::vector<Request> requests;
	requests.reserve(requestCount);
	for (std::size_t q = 0; q < requestCount; ++q)
	{
		requests.push_back(Request::Read(in));
	}

	return { values, requests };
}

OutputDataset ProcessData(InputDataset data)
{
	std::vector<long long> results;

	IntegerString str;
	for (auto value : data.Values)
	{
		str.PushBack(value);
	}

	for (auto request : data.Requests)
	{
		switch (request.Type)
		{
			case Sum:
				results.push_back(str.GetSumOnSubsegment(
					request.FirstPosition, request.SecondPosition));
				break;

			case Insert:
				str.Insert(request.FirstPosition, request.Value);
				break;

			case Remove:
				str.Remove(request.FirstPosition);
				break;

			case Set:
				str.SetOnSubsegment(
					request.FirstPosition, request.SecondPosition, request.Value);
				break;

			case Add:
				str.IncreaseOnSubsegment(
					request.FirstPosition, request.SecondPosition, request.Value);
				break;

			case NextPermutation:
				str.NextPermutationOnSubsegment(
					request.FirstPosition, request.SecondPosition);
				break;

			case PreviousPermutation:
				str.PreviousPermutationOnSubsegment(
					request.FirstPosition, request.SecondPosition);
				break;
		}
	}

	return { results, str };
}

void WriteData(OutputDataset data, std::ostream& out)
{
	for (auto value : data.Sums)
	{
		out << value << std::endl;
	}

	for (auto value : data.Leftovers)
	{
		out << value << " ";
	}
	out << std::endl;
}

void SolveTheTask(std::istream& in, std::ostream& out)
{
	WriteData(ProcessData(ReadData(in)), out);
}

int main()
{
	SolveTheTask(std::cin, std::cout);
	return 0;
} 
