#ifndef LIBSKIFF_BINARY_GENERATOR_HPP
#define LIBSKIFF_BINARY_GENERATOR_HPP

#include <cstdint>
#include <vector>

#include <libskiff/types.hpp>

namespace libskiff {
namespace generator {

//! \brief Generator interface
class binary_generator {
public:
  //! \brief Add a constant encoded from the instruction_generator
  //! \param data Encoded constant
  void add_constant(const libskiff::types::constant_type_e type,
                    const std::vector<uint8_t> data);

  //! \brief Add an instruction made by the instruction_generator
  //!        to the binary
  //! \param instruction The instruction to add to the binary
  void add_instruction(const std::vector<uint8_t> instruction);

  //! \brief Set debug level
  void set_debug(const libskiff::types::exec_debug_level_e level);

  //! \brief Generate the binary with the data given thus far
  //! \returns A skiff binary that will be compliant with the
  //!          compatibility constant
  [[nodiscard]] std::vector<uint8_t> generate_binary() const;

  //! \brief Set the entry address to the binary
  void set_entry(const uint64_t address);

  //! \brief Add a callable section
  void add_section(std::vector<uint8_t> encoded_section);

  //! \brief Add an interrupt section
  void add_interrupt(std::vector<uint8_t> encoded_interrupt);

private:
  // TODO: Add compile option to toggle this
  static constexpr bool is_experimental{false};

  std::vector<uint8_t> _constant_data;
  std::vector<uint8_t> _instruction_data;
  std::vector<uint8_t> _section_table;
  std::vector<uint8_t> _interrupt_table;
  uint64_t _number_of_constants{0};
  uint64_t _number_of_instructions{0};
  uint64_t _constant_address{0};
  uint64_t _num_sections{0};
  uint64_t _num_interrupts{0};
  uint64_t _entry{0};
  libskiff::types::exec_debug_level_e _debug{
      libskiff::types::exec_debug_level_e::NONE};
};

} // namespace generator
} // namespace libskiff

#endif
