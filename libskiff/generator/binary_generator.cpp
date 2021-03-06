#include <libskiff/generator/binary_generator.hpp>
#include <libskiff/bytecode/helpers.hpp>
#include <libskiff/version.hpp>

namespace libskiff {
namespace generator {

void binary_generator::add_constant(const libskiff::types::constant_type_e type,
                                    const std::vector<uint8_t> data)
{
  _constant_data.push_back(static_cast<uint8_t>(type));
  _constant_data.insert(_constant_data.end(), data.begin(), data.end());
  _number_of_constants++;
}

void binary_generator::add_instruction(const std::vector<uint8_t> instruction)
{
  _instruction_data.insert(_instruction_data.end(), instruction.begin(),
                           instruction.end());
  _number_of_instructions++;
}

void binary_generator::set_entry(const uint64_t address) { _entry = address; }

void binary_generator::set_debug(
    const libskiff::types::exec_debug_level_e level)
{
  _debug = level;
}

std::vector<uint8_t> binary_generator::generate_binary() const
{
  uint32_t compatibility{0};
  compatibility |= static_cast<uint32_t>(is_experimental ? 0xFF : 0x00) << 24;
  compatibility |=
      static_cast<uint32_t>(libskiff::version::semantic_version.major) << 16;
  compatibility |=
      static_cast<uint32_t>(libskiff::version::semantic_version.minor) << 8;
  compatibility |=
      static_cast<uint32_t>(libskiff::version::semantic_version.patch);

  auto encoded_compatibility =
      libskiff::bytecode::helpers::pack_4(compatibility);
  auto encoded_number_of_constants =
      libskiff::bytecode::helpers::pack_8(_number_of_constants);
  auto encoded_number_of_sections =
      libskiff::bytecode::helpers::pack_8(_num_sections);
  auto encoded_number_of_interrupts =
      libskiff::bytecode::helpers::pack_8(_num_interrupts);
  auto encoded_entry = libskiff::bytecode::helpers::pack_8(_entry);
  auto encoded_number_instructions =
      libskiff::bytecode::helpers::pack_8(_instruction_data.size());

  std::vector<uint8_t> binary;
  // Compatibility DWORD
  binary.insert(binary.end(), encoded_compatibility.begin(),
                encoded_compatibility.end());

  // Debug level
  binary.push_back(static_cast<uint8_t>(_debug));

  // Section Table
  binary.insert(binary.end(), encoded_number_of_sections.begin(),
                encoded_number_of_sections.end());
  binary.insert(binary.end(), _section_table.begin(), _section_table.end());

  // Interrupt Table
  binary.insert(binary.end(), encoded_number_of_interrupts.begin(),
                encoded_number_of_interrupts.end());
  binary.insert(binary.end(), _interrupt_table.begin(), _interrupt_table.end());

  // Constants
  binary.insert(binary.end(), encoded_number_of_constants.begin(),
                encoded_number_of_constants.end());
  binary.insert(binary.end(), _constant_data.begin(), _constant_data.end());

  // Entry QWORD
  binary.insert(binary.end(), encoded_entry.begin(), encoded_entry.end());

  // Num Instruction bytes QWORD
  binary.insert(binary.end(), encoded_number_instructions.begin(),
                encoded_number_instructions.end());

  // Instructuns
  binary.insert(binary.end(), _instruction_data.begin(),
                _instruction_data.end());
  return binary;
}

void binary_generator::add_section(std::vector<uint8_t> encoded_section)
{
  _num_sections++;
  _section_table.insert(_section_table.end(), encoded_section.begin(),
                        encoded_section.end());
}

void binary_generator::add_interrupt(std::vector<uint8_t> encoded_interrupt)
{
  _num_interrupts++;
  _interrupt_table.insert(_interrupt_table.end(), encoded_interrupt.begin(),
                          encoded_interrupt.end());
}

} // namespace generator
} // namespace libskiff