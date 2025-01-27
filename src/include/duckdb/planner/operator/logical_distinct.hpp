//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_distinct.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/logical_operator.hpp"
#include "duckdb/planner/bound_result_modifier.hpp"

namespace duckdb {

//! LogicalDistinct filters duplicate entries from its child operator
class LogicalDistinct : public LogicalOperator {
public:
	explicit LogicalDistinct(DistinctType distinct_type)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_DISTINCT), distinct_type(distinct_type) {
	}
	explicit LogicalDistinct(vector<unique_ptr<Expression>> targets, DistinctType distinct_type)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_DISTINCT), distinct_type(distinct_type),
	      distinct_targets(std::move(targets)) {
	}

	//! Whether or not this is a DISTINCT or DISTINCT ON
	DistinctType distinct_type;
	//! The set of distinct targets
	vector<unique_ptr<Expression>> distinct_targets;
	//! The order by modifier (optional, only for distinct on)
	unique_ptr<BoundOrderModifier> order_by;

public:
	string ParamsToString() const override;

	vector<ColumnBinding> GetColumnBindings() override {
		return children[0]->GetColumnBindings();
	}
	void Serialize(FieldWriter &writer) const override;
	static unique_ptr<LogicalOperator> Deserialize(LogicalDeserializationState &state, FieldReader &reader);

protected:
	void ResolveTypes() override {
		types = children[0]->types;
	}
};
} // namespace duckdb
