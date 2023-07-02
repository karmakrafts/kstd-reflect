/**
 * @author Alexander Hinze
 * @since 23/03/2023
 */

#pragma once

#include <kstd/defaults.hpp>
#include <kstd/meta.hpp>
#include <string>

#include "reflection_fwd.hpp"
#include "rtti.hpp"

namespace kstd::reflect {
    struct alignas(1) FunctionFlags {
        [[maybe_unused]] bool is_virtual : 1;
        [[maybe_unused]] bool is_override : 1;
        [[maybe_unused]] bool is_final : 1;
        [[maybe_unused]] bool is_noexcept : 1;
        [[maybe_unused]] bool is_leading_const : 1;
        [[maybe_unused]] bool is_trailing_const : 1;
    };

    namespace {
        template<typename T, typename... R>
        inline auto parse_params(std::vector<const RTTI*>& param_types) noexcept {
            param_types.push_back(&*lookup<T>());

            if constexpr(sizeof...(R) > 0) {
                parse_params<R...>(param_types);
            }
        }

        template<typename R, typename... ARGS>
        inline auto parse_signature(const RTTI*& return_type, std::vector<const RTTI*>& param_types) noexcept {
            return_type = &*lookup<R>();
            param_types.reserve(sizeof...(ARGS));
            parse_params<ARGS...>(param_types);
        }
    }// namespace

    template<typename R, typename... ARGS>
    class FunctionInfo : public TypeInfo<R(ARGS...)> {
        protected:
        std::string _name;                    // NOLINT
        std::vector<const RTTI*> _param_types;// NOLINT
        const RTTI* _return_type;             // NOLINT
        FunctionFlags _flags;                 // NOLINT

        [[nodiscard]] inline auto strip_name(const std::string_view& name) noexcept -> std::string {
            std::string result(name);

            if(result.find("::") != std::string::npos) {// Name contains namespace qualifier(s)
                const auto index = result.find_last_of("::");
                result = result.substr(index + 1);// Strip off namespace qualifier(s)
            }

            return result;
        }

        private:
        inline auto parse_flags() noexcept {
            _flags.is_virtual = this->_type_name.find("virtual") != std::string::npos;
            _flags.is_override = this->_type_name.find("override") != std::string::npos;
            _flags.is_final = this->_type_name.find("final") != std::string::npos;
            _flags.is_noexcept = this->_type_name.find("noexcept") != std::string::npos;
            _flags.is_leading_const = this->_type_name.starts_with("const");
            _flags.is_trailing_const = this->_type_name.ends_with("const");
        }

        public:
        KSTD_DEFAULT_MOVE_COPY(FunctionInfo)

        FunctionInfo(std::string mangled_type_name, std::string type_name, const std::string_view& name) noexcept :
                TypeInfo<R(ARGS...)>(std::move(mangled_type_name), std::move(type_name)),
                _name(std::move(strip_name(name))),
                _param_types({}),
                _return_type(reinterpret_cast<const RTTI*>(&*lookup<void>())),// NOLINT
                _flags({}) {
            parse_flags();// Parse function flags
            parse_signature<R, ARGS...>(_return_type, _param_types);
        }

        ~FunctionInfo() noexcept override = default;

        [[nodiscard]] auto get_element_type() const noexcept -> ElementType override {
            return ElementType::FUNCTION;
        }

        [[nodiscard]] auto to_string() const noexcept -> std::string override {
            return fmt::format("{}:{}", _name, this->get_mangled_type_name());
        }

        [[nodiscard]] auto is_same(const RTTI& other) const noexcept -> bool override {
            return other.get_element_type() == ElementType::FUNCTION &&
                   *(other.template as_function<R, ARGS...>()) == *this;
        }

        [[nodiscard]] inline auto get_name() const noexcept -> const std::string& {
            return _name;
        }

        [[nodiscard, maybe_unused]] inline auto get_flags() const noexcept -> const FunctionFlags& {
            return _flags;
        }

        [[nodiscard]] inline auto get_return_type() const noexcept -> const TypeInfo<R>& {
            return *reinterpret_cast<const TypeInfo<R>*>(_return_type);// NOLINT
        }

        [[nodiscard, maybe_unused]] inline auto get_param_types() const noexcept -> const std::vector<const RTTI*>& {
            return _param_types;
        }

        template<usize INDEX>
        [[nodiscard]] inline auto get_param() const noexcept -> decltype(auto) {
            return *reinterpret_cast<const TypeInfo<meta::PackElement<INDEX, meta::Pack<ARGS...>>>*>(
                    _param_types[INDEX]);// NOLINT
        }

        [[nodiscard, maybe_unused]] inline constexpr auto get_param_count() const noexcept -> usize {
            return sizeof...(ARGS);
        }

        template<typename OR, typename... OARGS>
        [[nodiscard]] inline auto operator==(const FunctionInfo<OR, OARGS...>& other) const noexcept -> bool {
            return this->get_mangled_type_name() == other.get_mangled_type_name() && this->_name == other._name;
        }
    };
}// namespace kstd::reflect