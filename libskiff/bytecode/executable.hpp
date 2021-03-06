#ifndef LIBSKIFF_EXECUTABLE_HPP
#define LIBSKIFF_EXECUTABLE_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include <libskiff/types.hpp>

#include <iostream>

namespace libskiff {
namespace bytecode {

//! \brief A loaded item
class executable_c {
public:
  //! \brief Construct an executable item
  executable_c(const uint32_t compatibility)
      : _debug_level(libskiff::types::exec_debug_level_e::NONE),
        _compatiblity_dword(compatibility)
  {
  }

  //! \brief Returns true if loaded object was marked as experimental
  bool is_experimental() const { return (_compatiblity_dword >> 24) == 0xFF; }

  //! \brief Retrieve the semver baked into the binary
  libskiff::types::semver_t bytecode_semver() const { return _semver; }

  //! \brief Set the debug level
  void set_debug_level(libskiff::types::exec_debug_level_e level)
  {
    _debug_level = level;
  }

  //! \brief Set constants
  void set_constants(const std::vector<uint8_t> &c) { _constants = c; }

  //! \brief Set instructions
  void set_instructions(const std::vector<uint8_t> &c) { _instructions = c; }

  //! \brief Set the entry address to the executable
  void set_entry_address(const uint64_t address) { _entry_address = address; }

  //! \brief Adds a section and address
  //! \returns false iff the name is not unique
  bool add_section(const std::string &name, const uint64_t address);

  //! \brief Adds an interrupt id and address
  //! \returns false iff the id is not unique
  bool add_interrupt(const uint64_t id, const uint64_t address);

  //! \brief Retrieve the debug level for the executable
  libskiff::types::exec_debug_level_e get_debug_level() const
  {
    return _debug_level;
  }

  //! \brief Get the entry address
  uint64_t get_entry_address() const { return _entry_address; }

  //! \brief Attempt to get a section address from the library table
  //! \returns nullopt if the name
  std::optional<uint64_t> get_section_address(const std::string &name) const;

  //! \brief Get the constants
  std::vector<uint8_t> get_constants() const { return _constants; }

  //! \brief Get the instructions
  std::vector<uint8_t> get_instructions() const { return _instructions; }

  //! \brief Get the compatiblity DWORD
  uint32_t get_compatiblity() const { return _compatiblity_dword; }

  //! \brief Retrieve the semver that compiled the code
  libskiff::types::semver_t get_compatiblity_semver() const
  {
    return libskiff::types::semver_t{
        .major = static_cast<uint8_t>(_compatiblity_dword >> 16),
        .minor = static_cast<uint8_t>(_compatiblity_dword >> 8),
        .patch = static_cast<uint8_t>(_compatiblity_dword)};
  }

  //! \brief Get the interrupt table
  std::unordered_map<uint64_t, uint64_t> get_interrupt_table() const
  {
    return _interrupt_number_to_address;
  }

private:
  std::vector<uint8_t> _constants;
  std::vector<uint8_t> _instructions;
  uint32_t _compatiblity_dword;
  libskiff::types::semver_t _semver;
  uint64_t _entry_address{0};
  std::unordered_map<std::string, uint64_t> _section_table;
  libskiff::types::exec_debug_level_e _debug_level;
  std::unordered_map<uint64_t, uint64_t> _interrupt_number_to_address;
};

//! \brief Load a given binary
//! \param file The file to load
//! \returns Optional unique pointer to a executable_c object. If there are
//!          errors loading the file nullopt will be returned
std::optional<std::unique_ptr<executable_c>>
load_binary(const std::string &file);

} // namespace bytecode
} // namespace libskiff

#endif
