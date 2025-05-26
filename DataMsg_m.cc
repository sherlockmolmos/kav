//
// Generated file, do not edit! Created by opp_msgtool 6.1 from DataMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "DataMsg_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(DataMsg)

DataMsg::DataMsg(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

DataMsg::DataMsg(const DataMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

DataMsg::~DataMsg()
{
}

DataMsg& DataMsg::operator=(const DataMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataMsg::copy(const DataMsg& other)
{
    this->truelen = other.truelen;
    this->datalen = other.datalen;
    for (size_t i = 0; i < 8; i++) {
        this->data[i] = other.data[i];
    }
    for (size_t i = 0; i < 8; i++) {
        this->data2[i] = other.data2[i];
    }
}

void DataMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->truelen);
    doParsimPacking(b,this->datalen);
    doParsimArrayPacking(b,this->data,8);
    doParsimArrayPacking(b,this->data2,8);
}

void DataMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->truelen);
    doParsimUnpacking(b,this->datalen);
    doParsimArrayUnpacking(b,this->data,8);
    doParsimArrayUnpacking(b,this->data2,8);
}

int DataMsg::getTruelen() const
{
    return this->truelen;
}

void DataMsg::setTruelen(int truelen)
{
    this->truelen = truelen;
}

int DataMsg::getDatalen() const
{
    return this->datalen;
}

void DataMsg::setDatalen(int datalen)
{
    this->datalen = datalen;
}

size_t DataMsg::getDataArraySize() const
{
    return 8;
}

char DataMsg::getData(size_t k) const
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)8, (unsigned long)k);
    return this->data[k];
}

void DataMsg::setData(size_t k, char data)
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)8, (unsigned long)k);
    this->data[k] = data;
}

size_t DataMsg::getData2ArraySize() const
{
    return 8;
}

char DataMsg::getData2(size_t k) const
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)8, (unsigned long)k);
    return this->data2[k];
}

void DataMsg::setData2(size_t k, char data2)
{
    if (k >= 8) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)8, (unsigned long)k);
    this->data2[k] = data2;
}

class DataMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_truelen,
        FIELD_datalen,
        FIELD_data,
        FIELD_data2,
    };
  public:
    DataMsgDescriptor();
    virtual ~DataMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(DataMsgDescriptor)

DataMsgDescriptor::DataMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(DataMsg)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

DataMsgDescriptor::~DataMsgDescriptor()
{
    delete[] propertyNames;
}

bool DataMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DataMsg *>(obj)!=nullptr;
}

const char **DataMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *DataMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int DataMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int DataMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_truelen
        FD_ISEDITABLE,    // FIELD_datalen
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_data
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_data2
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *DataMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "truelen",
        "datalen",
        "data",
        "data2",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int DataMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "truelen") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "datalen") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "data") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "data2") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *DataMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_truelen
        "int",    // FIELD_datalen
        "char",    // FIELD_data
        "char",    // FIELD_data2
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **DataMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DataMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DataMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        case FIELD_data: return 8;
        case FIELD_data2: return 8;
        default: return 0;
    }
}

void DataMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'DataMsg'", field);
    }
}

const char *DataMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DataMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        case FIELD_truelen: return long2string(pp->getTruelen());
        case FIELD_datalen: return long2string(pp->getDatalen());
        case FIELD_data: return long2string(pp->getData(i));
        case FIELD_data2: return long2string(pp->getData2(i));
        default: return "";
    }
}

void DataMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        case FIELD_truelen: pp->setTruelen(string2long(value)); break;
        case FIELD_datalen: pp->setDatalen(string2long(value)); break;
        case FIELD_data: pp->setData(i,string2long(value)); break;
        case FIELD_data2: pp->setData2(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataMsg'", field);
    }
}

omnetpp::cValue DataMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        case FIELD_truelen: return pp->getTruelen();
        case FIELD_datalen: return pp->getDatalen();
        case FIELD_data: return pp->getData(i);
        case FIELD_data2: return pp->getData2(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'DataMsg' as cValue -- field index out of range?", field);
    }
}

void DataMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        case FIELD_truelen: pp->setTruelen(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_datalen: pp->setDatalen(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_data: pp->setData(i,omnetpp::checked_int_cast<char>(value.intValue())); break;
        case FIELD_data2: pp->setData2(i,omnetpp::checked_int_cast<char>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataMsg'", field);
    }
}

const char *DataMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr DataMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void DataMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    DataMsg *pp = omnetpp::fromAnyPtr<DataMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

