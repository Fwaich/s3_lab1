#pragma once

template <typename T>
class Lazy_Sequence;

template <typename T>
class Generator
{
public:
    virtual T get_next() = 0;
    virtual bool has_next() = 0;
    virtual ~Generator() = default;
};

template <typename T>
class Function_Generator : public Generator<T>
{
private: 
    Lazy_Sequence<T>* owner;
    Array_Sequence<T> args_buffer; //нет необходимости хранить в полях тк постоянно перезаполоняется (кольцевой массив)
    std::function<T(Sequence<T>*)> rule;

public:
    Function_Generator(Lazy_Sequence<T>* owner, size_t arity, std::function<T(const Sequence<T>&)> rule) {
        this->args_buffer = Array_Sequence<T>(arity);
        this->owner = owner;
        this->rule = rule;
    }
    
    ~Function_Generator() {}

    T get_next() override {
        if (owner->get_materialized_count() < args_buffer.get_size())
            throw std::runtime_error("Not enough elements to generate next");

        size_t size = owner->get_materialized_count();
        for (size_t i = 0; i < args_buffer.get_size(); i++) {
            args_buffer.set(i, owner->get(size - 1 - i));
        }
        T item = rule(args_buffer);
        return item;
    }

    bool has_next() {
        return true;  //лиюо сделать как предел генерации
    }

};

template <typename T>
class Sequence_Generator : public Generator<T> 
{
private:
    Sequence<T>* seq; //подумать может ли быть lazy_seq
    size_t current_index;

public:
    Sequence_Generator(Sequence<T>* seq){ 
        this->seq = seq;
        this->current_index = 0;
    } 

    Sequence_Generator(Sequence<T>* seq, size_t current_index){ 
        this->seq = seq;
        this->current_index = current_index;
    } 

    ~Sequence_Generator() {
        delete seq;
    }

    T get_next() override {
        return seq->get(current_index++);
    }

    bool has_next() override {
        return current_index < seq->get_size();
    }
};

template <typename T>
class Concat_Generator : public Generator<T> 
{
private:
    Generator<T>* first;
    Generator<T>* second;

public:
    Concat_Generator(Generator<T>* primary_generator, Generator<T>* secondary_generator) {
        this->first = primary_generator;
        this->second = secondary_generator;
    }

    ~Concat_Generator() { //SMART_PTRS!!!
        delete first;
        delete second;
    }

    T get_next() override {
        if (!first->has_next()) return second->get_next();
        return first->get_next();
    }

    bool has_next() override {
        return first->has_next() || second->has_next();
    }
};

template <typename T>
class Insert_Generator : public Generator<T> 
{
private:
    Generator<T>* primary;
    Generator<T>* secondary;
    int current_index;
    int insert_index;

public:
    Insert_Generator(Generator<T>* primary_generator, Generator<T>* secondary_generator, 
        int insert_index, int current_index) {

        this->primary = primary_generator;
        this->secondary = secondary_generator;
        this->insert_index = insert_index;
        this->current_index = current_index;
    }

    ~Insert_Generator() {
        delete primary;
        delete secondary;
    }

    T get_next() override {
        if (current_index >= insert_index - 1 && secondary->has_next()) {
            current_index++;
            return secondary->get_next();
        }
        
        current_index++;
        return primary->get_next(); 
    }

    bool has_next() override {
        return primary->has_next() || secondary->has_next();
    }
};