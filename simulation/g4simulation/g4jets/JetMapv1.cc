#include "JetMapv1.h"

#include "Jet.h"

#include <cmath>

using namespace std;

JetMapv1::JetMapv1()
  : _algo(Jet::NONE)
  , _par(NAN)
  , _src()
  , _map()
{
}

JetMapv1::JetMapv1(const JetMapv1& jets)
  : _algo(jets.get_algo())
  , _par(jets.get_par())
  , _src()
  , _map()
{
  for (ConstSrcIter iter = jets.begin_src();
       iter != jets.end_src();
       ++iter)
  {
    _src.insert(*iter);
  }

  for (ConstIter iter = jets.begin();
       iter != jets.end();
       ++iter)
  {
    Jet* jet = (iter->second)->Clone();
    _map.insert(make_pair(jet->get_id(), jet));
  }
}

JetMapv1& JetMapv1::operator=(const JetMapv1& jets)
{
  Reset();

  _algo = jets.get_algo();
  _par = jets.get_par();

  for (ConstSrcIter iter = jets.begin_src();
       iter != jets.end_src();
       ++iter)
  {
    _src.insert(*iter);
  }

  for (ConstIter iter = jets.begin();
       iter != jets.end();
       ++iter)
  {
    Jet* jet = (iter->second)->Clone();
    _map.insert(make_pair(jet->get_id(), jet));
  }

  return *this;
}

JetMapv1::~JetMapv1()
{
  JetMapv1::Reset();
}

void JetMapv1::Reset()
{
  _algo = Jet::NONE;
  _par = NAN;
  _src.clear();

  while (_map.begin() != _map.end())
  {
    delete _map.begin()->second;
    _map.erase(_map.begin());
  }
}

JetMap* JetMapv1::Clone() const
{
  JetMap* map = new JetMapv1(*this);
  return map;
}

void JetMapv1::identify(ostream& os) const
{
  os << "JetMapv1: size = " << _map.size() << endl;
  os << "          par = " << _par << endl;
  os << "          source = ";
  for (ConstSrcIter i = begin_src(); i != end_src(); ++i)
  {
    os << (*i) << ",";
  }
  os << endl;

  return;
}

const Jet* JetMapv1::get(unsigned int id) const
{
  ConstIter iter = _map.find(id);
  if (iter == _map.end()) return nullptr;
  return iter->second;
}

Jet* JetMapv1::get(unsigned int id)
{
  Iter iter = _map.find(id);
  if (iter == _map.end()) return nullptr;
  return iter->second;
}

Jet* JetMapv1::insert(Jet* jet)
{
  unsigned int index = 0;
  if (!_map.empty()) index = _map.rbegin()->first + 1;
  _map.insert(make_pair(index, jet));
  _map[index]->set_id(index);
  return (_map[index]);
}
