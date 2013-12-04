#include "Sequence.hpp"
#include "DPMatcher.hpp"
#include "StraightMatcher.hpp"
#include <stdexcept>
#include <limits>
#include <iostream>

using namespace FireflyAssembler;
using namespace std;

void Sequence::free()
{

}

void Sequence::copy(const Sequence & other)
{
    cout << "Sequence size: " << other.sequence.size() << endl;
    sequence = other.sequence;
    matcher.reset(other.matcher.get());
}

Sequence::Sequence(MatcherConstPointer m) : sequence(), matcher(m)
{
}

Sequence::Sequence(MatcherConstPointer m, const char * s) : sequence(), matcher(m)
{
    while (*s)
    {
        sequence.push_back(*s++);
    }
}

Sequence::Sequence(MatcherConstPointer m, const string & s)  : sequence(), matcher(m)
{
    sequence.insert(sequence.begin(),
            s.begin(),
            s.end());
}

Sequence::Sequence(const Sequence & other) : sequence(other.sequence), matcher(other.matcher)
{
}

Sequence & Sequence::operator = (const Sequence & other)
{
    if (this != &other)
    {
        free();
        copy(other);
    }
}

Sequence::~Sequence()
{
    free();
}

void Sequence::append(const Sequence & other)
{
    sequence.insert(sequence.end(),
            other.sequence.begin(),
            other.sequence.end());
}

void Sequence::append(const string & s)
{
    sequence.insert(sequence.end(),
            s.begin(),
            s.end());
}


void Sequence::merge(const Sequence & other,
        int overlap)
{
    //if (overlap > other.sequence.size())
    //{
    //    throw runtime_error("overlap exceeds other sequence argument's length");
    //}
    if (overlap > sequence.size())
    {
        throw runtime_error("overlap exceeds this sequence's length");
    }

    vector<char> oldSequence = sequence;
    sequence.clear();
    sequence.insert(sequence.end(),
            oldSequence.begin(),
            oldSequence.begin() + (oldSequence.size() - overlap));
    sequence.insert(sequence.end(),
            other.sequence.begin(),
            other.sequence.end());
}


int Sequence::containsSize(const Sequence & other) const
{
    return matcher->containSize(this->sequence, other.sequence);
}

int Sequence::determineOverlap(const Sequence & other) const
{
    return matcher->overlapSize(this->sequence, other.sequence);
}

int Sequence::length() const
{
    return sequence.size();
}

void Sequence::clear()
{
    sequence.clear();
}

char Sequence::operator [](int index) const
{
    return sequence[index];
}

StringPointer Sequence::str() const
{
    return StringPointer(new string(sequence.begin(),
            sequence.end()));
}

bool Sequence::operator == (const Sequence & other) const
{
    return sequence == other.sequence;
}

bool Sequence::operator < (const Sequence & other) const
{
    return sequence < other.sequence;
}

ostream & FireflyAssembler::operator << (ostream & o,
        const Sequence & s)
{
    for (int i = 0; i < s.length(); i++)
    {
        o << s[i];
    }
    return o;
}
