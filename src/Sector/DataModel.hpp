#include <absl/strings/str_cat.h>

#include <concepts>
#include <functional>
#include <tl/expected.hpp>
#include <tuple>
#include <type_traits>

#include "Airport.hpp"
#include "Complement.hpp"
#include "Definition.hpp"
#include "file/FieldFormat.hpp"
#include "file/Parser.hpp"
#include "refl.hpp"

namespace sec {

using FltNumType = std::uint32_t;
using LegNoType = std::uint8_t;
using EqpCd = std::uint16_t;
using RuleSetCompIdx = std::uint8_t;

enum class RegionType : char { kDom = 'D', kInt = 'I', kReg = 'R' };

enum class OprType : std::uint8_t {
    kOpr,
    kDhd,
};

struct SecData {
    chr::TpMinsUnixEpoch dep_dt_utc, arr_dt_utc;
    FltNumType fn, next_fn;
    LegNoType leg_no;
    EqpCd eqp_cd;
    arp::Airport dep_arp, arr_arp;
    Complement comp;
    RuleSetCompIdx comp_idx;  // to be fixed
    OprType opr_type;
    RegionType reg_type;
    file::LineSvIdxType sec_svs_idx;
};

template <class DataStruct, template <class> class ParsingFunc>
tl::expected<DataStruct, file::parser::ParserError> Unserialize(
    std::string_view line, file::LineSvIdxType sec_svs_idx);

template <template <class> class ParsingFunc, class ParsingSigTuple>
concept LineFieldParsingFuncConcept =
    // std::invocable<ParsingFunc<ParsingSig...>, std::string_view> &&
    std::convertible_to<
        std::invoke_result_t<ParsingFunc<ParsingSigTuple>, std::string_view>,
        tl::expected<std::tuple_element_t<0, ParsingSigTuple>,
                     file::parser::ParserError>>;

template <class ReductionOp, class... ParsingSigTuples>
struct DataFieldProperty : public refl::attr::usage::field {
    ReductionOp reduction_op;
    std::tuple<ParsingSigTuples...> tuple_of_parsing_sig_tuples;
};

template <class ParsingSigTuple, class... Empty>
struct DataFieldProperty<std::tuple<ParsingSigTuple>, Empty...> final
    : public refl::attr::usage::field {
    util::PassThruFunctor<> reduction_op;
    std::tuple<ParsingSigTuple> tuple_of_parsing_sig_tuples;
};

auto ReturnArrTp = [&sec_data_instance = std::as_const(sec_data_instance)](
                       auto&& days_offset_local, auto&& arr_hhmm_local, auto&& blk_mins) {
    if (sec_data_instance.dep_dt_utc + blk_mins == sec_data_instance.dep_dt_utc + sec_data_instance.dep_arp.utc_offset_duration + days_offset_local + arr_hhmm_local) {
        return 
    }
};

}  // namespace sec

REFL_AUTO(
    type(sec::SecData),
    field(
        fn,
        sec::DataFieldProperty<std::tuple<
            decltype(sec::SecData::fn), util::int_c<file::SecLineField::kFltNo>,
            util::int_c<file::parser::Pattern::kConvertAlnumToUint>>>{}),
    field(next_fn,
          sec::DataFieldProperty<std::tuple<
              decltype(sec::SecData::next_fn),
              util::int_c<file::SecLineField::kNextFltNo>,
              util::int_c<file::parser::Pattern::kConvertAlnumToUint>>>{}),
    field(leg_no,
          sec::DataFieldProperty<
              std::tuple<decltype(sec::SecData::leg_no),
                         util::int_c<file::SecLineField::kLegNo>,
                         util::int_c<file::parser::Pattern::kParseNum>>>{}),
    field(eqp_cd,
          sec::DataFieldProperty<std::tuple<
              decltype(sec::SecData::eqp_cd),
              util::int_c<file::SecLineField::kEqpCd>,
              util::int_c<file::parser::Pattern::kConvertAlnumToUint>>>{}),
    field(dep_dt_utc,
          sec::DataFieldProperty<
              std::plus<>,
              std::tuple<chr::TpDaysUnixEpoch,
                         util::int_c<file::SecLineField::kDepYmd>,
                         util::int_c<file::parser::Pattern::kChronoDirect>>,
              std::tuple<chr::DurationMins,
                         util::int_c<file::SecLineField::kDepHhMm>,
                         util::int_c<file::parser::Pattern::kChronoDirect>>>{}),
    field(arr_dt_utc,
          sec::DataFieldProperty<
              decltype(),
              std::tuple<
                  chr::DurationDays,
                  util::int_c<file::SecLineField::kArrDayOffset>,
                  util::int_c<file::parser::Pattern::kChronoSignedDuration>>,
              std::tuple<chr::DurationMins,
                         util::int_c<file::SecLineField::kArrHhMm>,
                         util::int_c<file::parser::Pattern::kChronoDirect>>,
              std::tuple<chr::DurationMins,
                         util::int_c<file::SecLineField::kBlkMins>,
                         util::int_c<file::parser::Pattern::kChronoDirect>>>{}),
    field(eqp_cd,
          sec::DataFieldProperty<std::tuple<
              decltype(sec::SecData::eqp_cd),
              util::int_c<file::SecLineField::kEqpCd>,
              util::int_c<file::parser::Pattern::kConvertAlnumToUint>>>{}),
    field(
        dep_arp,
        sec::DataFieldProperty<
            util::PassThruFunctor<arp::Airport>,
            std::tuple<arp::ArpCdType,
                       util::int_c<file::SecLineField::kDepArpCd>,
                       util::int_c<file::parser::Pattern::kConvertAlnumToUint>>,
            std::tuple<arp::UtcOffsetDurType,
                       util::int_c<file::SecLineField::kDepArpUtcOffsetHhMm>,
                       util::int_c<file::parser::Pattern::kChronoDirect>>>{}),
    field(
        arr_arp,
        sec::DataFieldProperty<
            util::PassThruFunctor<arp::Airport>,
            std::tuple<arp::ArpCdType,
                       util::int_c<file::SecLineField::kArrArpCd>,
                       util::int_c<file::parser::Pattern::kConvertAlnumToUint>>,
            std::tuple<arp::UtcOffsetDurType,
                       util::int_c<file::SecLineField::kArrArpUtcOffsetHhMm>,
                       util::int_c<file::parser::Pattern::kChronoDirect>>>{}))

namespace sec {
template <class DataStruct, template <class> class ParsingFunc>
constexpr tl::expected<DataStruct, file::parser::ParserError> Unserialize(
    auto&& line, file::LineSvIdxType sec_svs_idx) {
    DataStruct data_instance;
    refl::util::for_each(
        refl::reflect<DataStruct>().members,
        [&data_instance,
         line = std::forward<decltype(line)>(line)](auto data_field) {
            constexpr auto&& property = std::get<0>(data_field.attributes);
            auto&& field_expected = std::invoke(
                [property = std::forward<decltype(property)>(property)](
                    auto&& line, auto&&... parsing_sig_tuple_idxs) {
                    return std::invoke(
                        property.reduction_op,
                        ParsingFunc<std::tuple_element_t<
                            parsing_sig_tuple_idxs,
                            decltype(property.tuple_of_parsing_sigs)>>(
                            std::forward<decltype(line)>(line))...);
                },
                std::forward<line>(line),
                std::index_sequence_for<
                    decltype(property.property.tuple_of_parsing_sigs)>{});
            data_field(data_instance) = field_expected ? *field_expected : 
        });
    return data_instance;
}  // namespace detail