#pragma once

#include <stdexcept>

class ConflictError: public std::runtime_error{
public:
    using std::runtime_error::runtime_error;
};

class DatabaseError: public std::runtime_error{

public:
    using std::runtime_error::runtime_error;
};

class InvalidJson: public std::runtime_error{

public:
    using std::runtime_error::runtime_error;
};

class ValidationError: public std::runtime_error{

public:
    using std::runtime_error::runtime_error;
};