#pragma once
#include <string>
#include <unordered_map>

class Material {
public:
    enum class Type {
        Aluminium,
        Steel,
        Brass,
        Copper
    };

private:
    Type type_;
    float E_;    // Young's modulus (Pa)
    float rho_;  // density (kg/m^3)
    float nu_;   // Poisson ratio
    float h_;    // thickness (m)
    std::string name_;

    void setProperties(Type t) {
        switch(t) {
            case Type::Aluminium:
                name_ = "Aluminium"; E_ = 7e10f; rho_ = 2700.0f; nu_ = 0.33f; h_ = 0.005f; break;
            case Type::Steel:
                name_ = "Steel";    E_ = 2e11f; rho_ = 7850.0f; nu_ = 0.30f; h_ = 0.005f; break;
            case Type::Brass:
                name_ = "Brass";    E_ = 1e11f; rho_ = 8500.0f; nu_ = 0.35f; h_ = 0.005f; break;
            case Type::Copper:
                name_ = "Copper";   E_ = 1.1e11f; rho_ = 8960.0f; nu_ = 0.34f; h_ = 0.005f; break;
        }
    }

public:
    Material(Type t) : type_(t) { setProperties(t); }

    // getters
    Type type() const { return type_; }
    const std::string& name() const { return name_; }
    float E() const { return E_; }
    float rho() const { return rho_; }
    float nu() const { return nu_; }
    float h() const { return h_; }

    // bending stiffness
    float getD() const {
        return E_ * h_*h_*h_ / (12.0f * (1.0f - nu_*nu_));
    }

    // material force factor
    float getMaterialForce() const {
        return 100.0f * getD() / (rho_ * h_);
    }
};
