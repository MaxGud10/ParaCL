1.
// Создание нод в целом хорошее, но небольшие улучшения не помешают.
// 1. Не создавай ноды напрямую. Используй для этого builder:
//     bld.create<AST::ScopeNode>( args...);
//    Например, для бинарной ноды я ожидаю создание примерно так:
//     $$ = bld.create<AST::BinaryOpNode>($1, AST::BinaryOp::LS, $3);
//    И больше ничего в .y файле.
//    Логирование тоже можно перенести в C++-файлы.
// 2. Не используй std::unique_ptr как саму ноду. Ты перекладываешь на ноду слишком много ответственности.
//    За владение памятью и ресурсами должен отвечать специальный класс (возможно, тот же builder).
// 3. Сделай уничтожение AST нерекурсивным. Это будет легко после того, как пункт 2 будет сделан.
drv.ast.globalScope =
        std::make_unique<AST::ScopeNode>(std::move(drv.stm_table[drv.cur_scope_id]));

2.
// Можно ли сделать это через уже существующие операции?
// x += 5 <=> x = x + 5 <=> бинарная операция + присваивание
$$ = std::make_unique<AST::AssignNode>( std::move($1),


3. 
// Эта VarTable владеет именами переменных через std::string, и VariableNode тоже владеет именем через std::string.
// Должен быть только один владелец ресурса имени. Нужно выбрать: это VarTable или VariableNode.
// Остальные места должны использовать std::string_view.
using VarTable = std::unordered_map<std::string, int>;


4. 
// Возможны проблемы с ODR
const size_t numBinaryOp = 20;


5. 
// Ты пометил метод как const, но при этом переместил (move) состояние children. Почему?
// Общий совет: не используй const_cast. В большинстве случаев это будет ошибкой.
std::vector<StmtPtr> get_children() const { return std::move(const_cast<std::vector<StmtPtr>&>(children_)); ; }


6. 
// Если параметр не используется, можно не давать ему имя:
// int eval(detail::Context& ) const override;
// Сейчас я не буду придираться к этой аннотации, но на будущее:
// Если это часто требуется — возможно, дизайн нуждается в рефакторинге.
int eval([[maybe_unused]]detail::Context& ctx) const override
{


7. 
// for (auto &&VarTable : std::reverse(ctx.varTables_)) ...
for (int32_t scopeId = ctx.curScope_; scopeId >= 0; --scopeId)


8. 
// В C/C++ условия вычисляются лениво (short-circuit):
// Поэтому если leftVal — false, нет нужды вычислять rightVal.
// Пожалуйста, реализуй такое же поведение.
case BinaryOp::AND:
    result = leftVal && rightVal;


9. 
// ну да, “blya” действительно :)
LOG("Blya: unexpected AssignType {} in switch\n",
    static_cast<int>(type_));


10. 
// Используй алгоритмы
// auto It = std::find_if(std::reverse(ctx.varTables_), ...)
while (scopeId < ctx.curScope_)
{
    if (ctx.varTables_[static_cast<std::size_t>(scopeId)].contains(destName))


11. 
// В ctx можно хранить указатель на текущий scope, или сделать метод, который возвращает текущий scope.
// Не вводи scopeId вне ctx — его придётся вручную поддерживать, что плохо.
ctx.varTables_[static_cast<std::size_t>(scopeId)][destName] = value;


12. 
// Почему ты переписал цикл `for` через `while`?
if (init_)
    init_->eval(ctx);
while (cond_->eval(ctx))


13. 
// Скорее всего эти определения в заголовке вызовут нарушение ODR
// Также лучше использовать constexpr
const std::string SET_NODE              = "node";


14. 
// Используй std::string_view вместо `const std::string &`
std::string generateFileName(const std::string& prefix, const std::string& extension);


15. 
// Может быть constexpr-переменные внутри namespace?
enum NodeColors {
    VARIABLE_NODE_COLOR     = 0xC2EEC7,
