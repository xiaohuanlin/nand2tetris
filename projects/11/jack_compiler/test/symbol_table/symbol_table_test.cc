#include "exception/exception.h"
#include "utils/enum/enum.h"
#include "symbol_table/symbol_table.h"

#include "gtest/gtest.h"

namespace jack_compiler {
TEST(SymbolTest, NormalTest) {
    SymbolTable symbol_table;
    symbol_table.StartSubroutine();

    symbol_table.Define("one", "int", SymbolTableKind::kStatic);
    symbol_table.Define("two", "boolean", SymbolTableKind::kField);
    symbol_table.Define("three", "Bank", SymbolTableKind::kVar);
    symbol_table.Define("four", "Art", SymbolTableKind::kArg);
    symbol_table.Define("five", "Art", SymbolTableKind::kField);

    ASSERT_EQ(1, symbol_table.VarCount(SymbolTableKind::kStatic));
    ASSERT_EQ(2, symbol_table.VarCount(SymbolTableKind::kField));
    ASSERT_EQ(1, symbol_table.VarCount(SymbolTableKind::kVar));
    ASSERT_EQ(1, symbol_table.VarCount(SymbolTableKind::kArg));

    ASSERT_EQ(SymbolTableKind::kStatic, symbol_table.KindOf("one"));
    ASSERT_EQ("int", symbol_table.TypeOf("one"));
    ASSERT_EQ(0, symbol_table.IndexOf("one"));
    ASSERT_EQ(SymbolTableKind::kField, symbol_table.KindOf("five"));
    ASSERT_EQ("Art", symbol_table.TypeOf("five"));
    ASSERT_EQ(1, symbol_table.IndexOf("five"));

    symbol_table.StartSubroutine();
    EXPECT_THROW({
        symbol_table.KindOf("four");
    }, CompileException);
    ASSERT_EQ(SymbolTableKind::kStatic, symbol_table.KindOf("one"));
}
} //  jack_compiler