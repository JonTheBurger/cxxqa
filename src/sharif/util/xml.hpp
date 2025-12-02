/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std

// 3rd
#include <pugixml.hpp>

// local
#include <sharif/util/result.hpp>

// namespace
namespace sharif::xml {

/* Types
 ******************************************************************************/
using Document = pugi::xml_document;
using Node     = pugi::xml_node;
using Code     = pugi::xml_parse_status;
using Error    = SHARIF_ERROR_NAMESPACE::quick_status_code_from_enum<Code>;
}  // namespace sharif::xml

namespace SHARIF_ERROR_NAMESPACE {
// NOLINTBEGIN(readability-identifier-naming)
template <>
struct quick_status_code_from_enum<sharif::xml::Code> : quick_status_code_from_enum_defaults<sharif::xml::Code> {
  static constexpr const auto domain_name = "xml";
  /// https://www.random.org/cgi-bin/randbyte?nbytes=16&format=h
  static constexpr const auto domain_uuid = "{4e2e6222-f459-37bd-c974-ddf561ada4d7}";

  static auto value_mappings() -> const std::initializer_list<mapping>&
  {
    static const std::initializer_list<mapping> errors = {
      { .value = pugi::status_ok, .message = "OK", .code_mappings = { sharif::Code::success } },
      { .value = pugi::status_file_not_found, .message = "File was not found during load_file()", .code_mappings = {} },
      { .value = pugi::status_io_error, .message = "Error reading from file/stream", .code_mappings = {} },
      { .value = pugi::status_out_of_memory, .message = "Could not allocate memory", .code_mappings = {} },
      { .value = pugi::status_internal_error, .message = "Internal error occurred", .code_mappings = {} },
      { .value = pugi::status_unrecognized_tag, .message = "Parser could not determine tag type", .code_mappings = {} },
      { .value = pugi::status_bad_pi, .message = "Parsing error occurred while parsing document declaration/processing instruction", .code_mappings = {} },
      { .value = pugi::status_bad_comment, .message = "Parsing error occurred while parsing comment", .code_mappings = {} },
      { .value = pugi::status_bad_cdata, .message = "Parsing error occurred while parsing CDATA section", .code_mappings = {} },
      { .value = pugi::status_bad_doctype, .message = "Parsing error occurred while parsing document type declaration", .code_mappings = {} },
      { .value = pugi::status_bad_pcdata, .message = "Parsing error occurred while parsing PCDATA section", .code_mappings = {} },
      { .value = pugi::status_bad_start_element, .message = "Parsing error occurred while parsing start element tag", .code_mappings = {} },
      { .value = pugi::status_bad_attribute, .message = "Parsing error occurred while parsing element attribute", .code_mappings = {} },
      { .value = pugi::status_bad_end_element, .message = "Parsing error occurred while parsing end element tag", .code_mappings = {} },
      { .value = pugi::status_end_element_mismatch, .message = "There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag)", .code_mappings = {} },
      { .value = pugi::status_append_invalid_root, .message = "Unable to append nodes since root type is not node_element or node_document (exclusive to xml_node::append_buffer)", .code_mappings = {} },
      { .value = pugi::status_no_document_element, .message = "Parsing resulted in a document without element nodes", .code_mappings = {} },
    };
    return errors;
  }
};

// NOLINTEND(readability-identifier-naming)
}  // namespace SHARIF_ERROR_NAMESPACE
