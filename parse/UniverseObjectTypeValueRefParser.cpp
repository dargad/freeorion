#include "ValueRefParserImpl.h"

#include "EnumParser.h"


namespace {
    struct universe_object_type_parser_rules {
        universe_object_type_parser_rules()  {
            qi::_1_type _1;
            qi::_val_type _val;
            using phoenix::new_;
            using phoenix::push_back;

            const parse::lexer& tok = parse::lexer::instance();

            variable_name
                %=   tok.ObjectType_
                ;

            constant
                =    parse::universe_object_type_enum() [ _val = new_<ValueRef::Constant<UniverseObjectType> >(_1) ]
                ;

            initialize_bound_variable_parser<UniverseObjectType>(bound_variable, variable_name);

            statistic_sub_value_ref
                =   constant
                |   bound_variable
                ;

            initialize_nonnumeric_expression_parsers<UniverseObjectType>(function_expr, operated_expr, expr, primary_expr);

            initialize_nonnumeric_statistic_parser<UniverseObjectType>(statistic, statistic_sub_value_ref);

            primary_expr
                =   constant
                |   bound_variable
                |   statistic
                ;

            variable_name.name("ObjectType variable name (e.g., ObjectType)");
            constant.name("ObjectType");
            bound_variable.name("ObjectType variable");
            statistic.name("ObjectType statistic");
            primary_expr.name("ObjectType expression");

#if DEBUG_VALUEREF_PARSERS
            debug(variable_name);
            debug(constant);
            debug(bound_variable);
            debug(statistic);
            debug(primary_expr);
#endif
        }

        typedef parse::value_ref_parser_rule<UniverseObjectType>::type  rule;
        typedef variable_rule<UniverseObjectType>::type                 variable_rule;
        typedef statistic_rule<UniverseObjectType>::type                statistic_rule;
        typedef expression_rule<UniverseObjectType>::type               expression_rule;

        name_token_rule variable_name;
        rule            constant;
        variable_rule   bound_variable;
        rule            statistic_sub_value_ref;
        statistic_rule  statistic;
        expression_rule function_expr;
        expression_rule operated_expr;
        rule            expr;
        rule            primary_expr;
    };
}

namespace parse {
    value_ref_parser_rule<UniverseObjectType>::type& universe_object_type_value_ref()
    {
        static universe_object_type_parser_rules retval;
        return retval.expr;
    }
}
