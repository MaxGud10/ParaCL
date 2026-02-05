Внутри нашей ноды мы хранили friend-функцию переопределенного оператора для распечатки в dot + саму функцию `dump`. Это более интрузивный подход и выглядит вот так:

```cpp
    friend std::ostream& operator<<(std::ostream& os, const VariableNode& n) {
        os << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << n.name_   << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::VARIABLE_NODE_COLOR << std::dec
        << END_NODE;

        return os;
    }

    void dump(std::ostream& os) const override {
        MSG("dumping variable node\n");
        os << *this;
    }
```

тут мы нарушаем OCP,SRP, а еще походу DIP (распечатка зависит от низкоуровневого класса std::ostream а не от абстракции). В целом это неудобно, так как если мы захотим добавить 10 распечаток, то придется добавить 10 таких friend-функций в для каждой ноды внутрь класса...

Для решения такой проблемы реализован базовый класс Visitor.
Внутри каждой ноды переопрелен метод accept.
```cpp
void accept(const Visitor& visitor) const override {
        visitor.VisitScopeNode(*this);
    }
```
Далее наследник Visitor - DotPrinter, где мы переопределяем: `VisitScopeNode, etc...`

```cpp
void VisitScopeNode(const AST::ScopeNode& n) const override {
        os_ << SET_NODE << &n
        << SET_MRECORD_SHAPE
        << SET_LABEL  << "SCOPE" << SET_ADR  << &n << END_LABEL
        << SET_FILLED << SET_COLOR << std::hex << AST::dump_style::SCOPE_NODE_COLOR << std::dec
        << END_NODE;

        const auto& children = n.get_children();
        for (const auto &child : children) {
            os_ << SET_NODE << &n << SET_LINK << SET_NODE << child << std::endl;
            child->accept(*this);
        }
    }
```

Теперь нам не нужно модифицировать класс inode, если мы хотим добавить новый сценарий visit'а. Для этого нам достаточно создать новый .hpp и переопределить методы.
