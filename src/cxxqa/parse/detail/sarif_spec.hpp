/** JSON Schema converted to C++ structs */
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <cstdint>
#include <flat_map>
#include <optional>
#include <string>
#include <vector>

// 3rd
#include <glaze/glaze.hpp>

// local
#include <cxxqa/util/heap_optional.hpp>

// namespace
// NOLINTBEGIN(readability-identifier-naming)
namespace cxxqa::sarif {

/* Constants
 ******************************************************************************/
constexpr std::string_view MIME_TYPE_PATTERN                = "[^/]+/.+";
constexpr std::string_view GUID_PATTERN                     = "^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[1-5][0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$";
constexpr std::string_view LANGUAGE_PATTERN                 = "^[a-zA-Z]{2}|^[a-zA-Z]{2}-[a-zA-Z]{2}?$";
constexpr std::string_view DOTTED_QUAD_FILE_VERSION_PATTERN = "[0-9]+(\\.[0-9]+){3}";
constexpr size_t           HEAP_OPTION_THRESHOLD            = 32;

/* Types
 ******************************************************************************/
template <typename T>
using optional_t = std::conditional_t<(sizeof(T) > HEAP_OPTION_THRESHOLD), heap_optional<T>, std::optional<T>>;

template <typename Key, typename Value>
using map_t = std::flat_map<Key, Value>;

/// The role or roles played by the artifact in the analysis.
enum class Role : uint8_t {
  analysisTarget,
  attachment,
  responseFile,
  resultFile,
  standardStream,
  tracedFile,
  unmodified,
  modified,
  added,
  deleted,
  renamed,
  uncontrolled,
  driver,
  extension,
  translation,
  taxonomy,
  policy,
  referencedOnCommandLine,
  memoryContents,
  directory,
  userSpecifiedConfiguration,
  toolSpecifiedConfiguration,
  debugOutputFile,
};

enum class Level : uint8_t {
  none,
  note,
  warning,
  error,
};

/// The state of a result relative to a baseline of a previous run.
enum class BaselineState : uint8_t {
  new_,
  unchanged,
  updated,
  absent,
};

/// Specifies the unit in which the tool measures columns.
enum class ColumnKind : uint8_t {
  utf16CodeUnits,
  unicodeCodePoints,
};

/// Specifies the importance of this location in understanding the code flow in which
/// it occurs. The order from most to least important is "essential", "important",
/// "unimportant". Default: "important".
enum class Importance : uint8_t {
  important,
  essential,
  unimportant,
};

/// The kinds of data contained in this object.
enum class LocalizedData : uint8_t {
  localizedData,
  nonLocalizedData,
};

/// Key/value pairs that provide additional information about the object.
struct PropertyBag {
  /// A set of distinct strings that provide additional information.
  optional_t<std::vector<std::string>> tags;

  /// Additional properties
  optional_t<map_t<std::string, glz::generic>> additionalProperties;
};

/// Identifies a particular toolComponent object, either the driver or an extension.
struct ToolComponentReference {
  /// The 'name' property of the referenced toolComponent.
  optional_t<std::string> name;

  /// An index into the referenced toolComponent in tool.extensions.
  optional_t<int32_t> index = -1;

  /// The 'guid' property of the referenced toolComponent.
  optional_t<std::string> guid;

  /// Key/value pairs that provide additional information about the toolComponentReference.
  optional_t<PropertyBag> properties;
};

/// A message string or message format string rendered in multiple formats.
struct MultiformatMessageString {
  /// A plain text message string or format string.
  std::string text;

  /// A Markdown message string or format string.
  optional_t<std::string> markdown;

  /// Key/value pairs that provide additional information about the message.
  optional_t<PropertyBag> properties;
};

/// Encapsulates a message intended to be read by the end user.
struct Message {
  /// A plain text message string.
  /// @warning Either this or `id` is required
  optional_t<std::string> text;

  /// A Markdown message string.
  optional_t<std::string> markdown;

  /// The identifier for this message.
  /// @warning Either this or `message` is required
  optional_t<std::string> id;

  /// An array of strings to substitute into the message string.
  optional_t<std::vector<std::string>> arguments;

  /// Key/value pairs that provide additional information about the message.
  optional_t<PropertyBag> properties;
};

/// Represents the contents of an artifact.
struct ArtifactContent {
  /// UTF-8-encoded content from a text artifact.
  optional_t<std::string> text;

  /// MIME Base64-encoded content from a binary artifact, or from a text artifact in its original encoding.
  optional_t<std::string> binary;

  /// An alternate rendered representation of the artifact (e.g., a decompiled representation of a binary region).
  optional_t<MultiformatMessageString> rendered;

  /// Key/value pairs that provide additional information about the artifact content.
  optional_t<PropertyBag> properties;
};

/// Specifies the location of an artifact.
struct ArtifactLocation {
  /// A string containing a valid relative or absolute URI.
  optional_t<std::string> uri;

  /// A string which indirectly specifies the absolute URI with respect to which a relative URI in the "uri" property is interpreted.
  optional_t<std::string> uriBaseId;

  /// The index within the run artifacts array of the artifact object associated with the artifact location.
  optional_t<int32_t> index = -1;

  /// A short description of the artifact location.
  optional_t<Message> description;

  /// Key/value pairs that provide additional information about the artifact location.
  optional_t<PropertyBag> properties;
};

/// An area within an image.
struct Rectangle {
  /// The Y coordinate of the top edge of the rectangle, measured in the image's natural units.
  double top;

  /// The X coordinate of the left edge of the rectangle, measured in the image's natural units.
  double left;

  /// The Y coordinate of the bottom edge of the rectangle, measured in the image's natural units.
  double bottom;

  /// The X coordinate of the right edge of the rectangle, measured in the image's natural units.
  double right;

  /// A message relevant to the rectangle.
  optional_t<Message> message;

  /// Key/value pairs that provide additional information about the rectangle.
  optional_t<PropertyBag> properties;
};

/// A region within an artifact where a result was detected.
struct Region {
  /// The line number of the first character in the region.
  optional_t<uint32_t> startLine;

  /// The column number of the first character in the region.
  optional_t<uint32_t> startColumn;

  /// The line number of the last character in the region.
  optional_t<uint32_t> endLine;

  /// The column number of the character following the end of the region.
  optional_t<uint32_t> endColumn;

  /// The zero-based offset from the beginning of the artifact of the first character in the region.
  optional_t<int32_t> charOffset = -1;

  /// The length of the region in characters.
  optional_t<int32_t> charLength;

  /// The zero-based offset from the beginning of the artifact of the first byte in the region.
  optional_t<int32_t> byteOffset = -1;

  /// The length of the region in bytes.
  optional_t<int32_t> byteLength;

  /// The portion of the artifact contents within the specified region.
  optional_t<ArtifactContent> snippet;

  /// A message relevant to the region.
  optional_t<Message> message;

  /// Specifies the source language, if any, of the portion of the artifact specified by the region object.
  optional_t<std::string> sourceLanguage;

  /// Key/value pairs that provide additional information about the region.
  optional_t<PropertyBag> properties;
};

/// A physical or virtual address, or a range of addresses, in an 'addressable region' (memory or a binary file).
struct Address {
  /// The address expressed as a byte offset from the start of the addressable region.
  optional_t<int64_t> absoluteAddress = -1;

  /// The address expressed as a byte offset from the absolute address of the top-most parent object.
  optional_t<int64_t> relativeAddress;

  /// The number of bytes in this range of addresses.
  optional_t<int32_t> length;

  /// An open-ended string that identifies the address kind. 'data', 'function', 'header','instruction', 'module', 'page', 'section', 'segment', 'stack', 'stackFrame', 'table' are well-known values.
  optional_t<std::string> kind;

  /// A name that is associated with the address, e.g., '.text'.
  optional_t<std::string> name;

  /// A human-readable fully qualified name that is associated with the address.
  optional_t<std::string> fullyQualifiedName;

  /// The byte offset of this address from the absolute or relative address of the parent object.
  optional_t<int32_t> offsetFromParent;

  /// The index within run.addresses of the cached object for this address.
  optional_t<int32_t> index = -1;

  /// The index within run.addresses of the parent object.
  optional_t<int32_t> parentIndex = -1;

  /// Key/value pairs that provide additional information about the address.
  optional_t<PropertyBag> properties;
};

/// A single artifact. In some cases, this artifact might be nested within another artifact.
struct Artifact {
  /// A short description of the artifact.
  optional_t<Message> description;

  /// The location of the artifact.
  optional_t<ArtifactLocation> location;

  /// Identifies the index of the immediate parent of the artifact, if this artifact is nested.
  optional_t<int32_t> parentIndex = -1;

  /// The offset in bytes of the artifact within its containing artifact.
  optional_t<int32_t> offset;

  /// The length of the artifact in bytes.
  optional_t<int64_t> length = -1;

  /// The role or roles played by the artifact in the analysis.
  optional_t<std::vector<Role>> roles;  // "analysisTarget", "attachment", "responseFile", etc.

  /// The MIME type (RFC 2045) of the artifact.
  optional_t<std::string> mimeType;

  /// The contents of the artifact.
  optional_t<ArtifactContent> contents;

  /// Specifies the encoding for an artifact object that refers to a text file.
  optional_t<std::string> encoding;

  /// Specifies the source language for any artifact object that refers to a text file that contains source code.
  optional_t<std::string> sourceLanguage;

  /// A dictionary, each of whose keys is the name of a hash function and each of whose values is the hashed value of the artifact produced by the specified hash function.
  optional_t<map_t<std::string, std::string>> hashes;

  /// The Coordinated Universal Time (UTC) date and time at which the artifact was most recently modified. See "Date/time properties" in the SARIF spec for the required format.
  optional_t<std::string> lastModifiedTimeUtc;

  /// Key/value pairs that provide additional information about the artifact.
  optional_t<PropertyBag> properties;
};

/// The replacement of a single region of an artifact.
struct Replacement {
  /// The region of the artifact to delete.
  Region deletedRegion;

  /// The content to insert at the location specified by the 'deletedRegion' property.
  optional_t<ArtifactContent> insertedContent;

  /// Key/value pairs that provide additional information about the replacement.
  optional_t<PropertyBag> properties;
};

/// A change to a single artifact.
struct ArtifactChange {
  /// The location of the artifact to change.
  ArtifactLocation artifactLocation;

  /// An array of replacement objects, each of which represents the replacement of a single region in a single artifact specified by 'artifactLocation'.
  std::vector<Replacement> replacements;

  /// Key/value pairs that provide additional information about the change.
  optional_t<PropertyBag> properties;
};

/// A proposed fix for the problem represented by a result object.
struct Fix {
  /// A message that describes the proposed fix, enabling viewers to present the proposed change to an end user.
  optional_t<Message> description;

  /// One or more artifact changes that comprise a fix for a result.
  std::vector<ArtifactChange> artifactChanges;

  /// Key/value pairs that provide additional information about the fix.
  optional_t<PropertyBag> properties;
};

/// An artifact relevant to a result.
struct Attachment {
  /// A message describing the role played by the attachment.
  optional_t<Message> description;

  /// The location of the attachment.
  ArtifactLocation artifactLocation;

  /// An array of regions of interest within the attachment.
  optional_t<std::vector<Region>> regions;

  /// An array of rectangles specifying areas of interest within the image.
  optional_t<std::vector<Rectangle>> rectangles;

  /// Key/value pairs that provide additional information about the attachment.
  optional_t<PropertyBag> properties;
};

/// A logical location of a construct that produced a result.
struct LogicalLocation {
  /// Identifies the construct in which the result occurred. For example, this property might contain the name of a class or a method.
  optional_t<std::string> name;

  /// The index within the logical locations array.
  optional_t<int32_t> index = -1;

  /// The human-readable fully qualified name of the logical location.
  optional_t<std::string> fullyQualifiedName;

  /// The machine-readable name for the logical location, such as a mangled function name provided by a C++ compiler that encodes calling convention, return type and other details along with the function name.
  optional_t<std::string> decoratedName;

  /// Identifies the index of the immediate parent of the construct in which the result was detected.
  optional_t<int32_t> parentIndex = -1;

  /// The type of construct this logical location component refers to. Should be one of 'function', 'member', 'module', 'namespace', 'parameter', 'resource', 'returnType', 'type', 'variable', etc.
  optional_t<std::string> kind;

  /// Key/value pairs that provide additional information about the logical location.
  optional_t<PropertyBag> properties;
};

/// Information about the relation of one location to another.
struct LocationRelationship {
  /// A reference to the related location.
  uint32_t target;

  /// A set of distinct strings that categorize the relationship. Well-known kinds include 'includes', 'isIncludedBy' and 'relevant'.
  optional_t<std::vector<std::string>> kinds = std::vector<std::string>{ "relevant" };

  /// A description of the location relationship.
  optional_t<Message> description;

  /// Key/value pairs that provide additional information about the location relationship.
  optional_t<PropertyBag> properties;
};

/// A physical location relevant to a result.
struct PhysicalLocation {
  /// The address of the location.
  /// @warning Either this or `artifactLocation` must be specified
  optional_t<Address> address;

  /// The location of the artifact.
  /// @warning Either this or `address` must be specified
  optional_t<ArtifactLocation> artifactLocation;

  /// Specifies a portion of the artifact.
  optional_t<Region> region;

  /// Specifies a portion of the artifact that encloses the region. Allows a viewer to display additional context around the region.
  optional_t<Region> context_region;

  /// Key/value pairs that provide additional information about the physical location.
  optional_t<PropertyBag> properties;
};

/// A location within a programming artifact.
struct Location {
  /// Value that distinguishes this location from all other locations within a single result object.
  optional_t<int32_t> id = -1;

  /// Identifies the artifact and region.
  optional_t<PhysicalLocation> physicalLocation;

  /// The logical locations associated with the result.
  optional_t<std::vector<LogicalLocation>> logicalLocations;

  /// A message relevant to the location.
  optional_t<Message> message;

  /// A set of regions relevant to the location.
  optional_t<std::vector<Region>> annotations;

  /// An array of objects that describe relationships between this location and others.
  optional_t<std::vector<LocationRelationship>> relationships;

  /// Key/value pairs that provide additional information about the location.
  optional_t<PropertyBag> properties;
};

/// Represents a node in a graph.
struct Node {
  /// A string that uniquely identifies the node within its graph.
  std::string id;

  /// A short description of the node.
  optional_t<Message> label;

  /// A code location associated with the node.
  optional_t<Location> location;

  /// Array of child nodes.
  optional_t<std::vector<Node>> children;

  /// Key/value pairs that provide additional information about the node.
  optional_t<PropertyBag> properties;
};

/// Represents a directed edge in a graph.
struct Edge {
  /// A string that uniquely identifies the edge within its graph.
  std::string id;

  /// A short description of the edge.
  optional_t<Message> label;

  /// Identifies the source node (the node at which the edge starts).
  std::string sourceNodeId;

  /// Identifies the target node (the node at which the edge ends).
  std::string targetNodeId;

  /// Key/value pairs that provide additional information about the edge.
  optional_t<PropertyBag> properties;
};

/// A network of nodes and directed edges that describes some aspect of the structure of the code.
struct Graph {
  /// A description of the graph.
  optional_t<Message> description;

  /// An array of node objects representing the nodes of the graph.
  optional_t<std::vector<Node>> nodes;

  /// An array of edge objects representing the edges of the graph.
  optional_t<std::vector<Edge>> edges;

  /// Key/value pairs that provide additional information about the graph.
  optional_t<PropertyBag> properties;
};

/// Represents the traversal of a single edge during a graph traversal.
struct EdgeTraversal {
  /// Identifies the edge being traversed.
  std::string edgeId;

  /// A message to display to the user as the edge is traversed.
  optional_t<Message> message;

  /// The values of relevant expressions after the edge has been traversed.
  optional_t<map_t<std::string, MultiformatMessageString>> finalState;

  /// The number of edge traversals necessary to return from a nested graph.
  optional_t<int32_t> stepOverEdgeCount;

  /// Key/value pairs that provide additional information about the edge traversal.
  optional_t<PropertyBag> properties;
};

/// Represents a path through a graph.
struct GraphTraversal {
  /// The index within the run.graphs to be associated with the result.
  /// @warning Either this or `resultGraphIndex` is required
  optional_t<int32_t> runGraphIndex = -1;

  /// The index within the result.graphs to be associated with the result.
  /// @warning Either this or `runGraphIndex` is required
  optional_t<int32_t> resultGraphIndex = -1;

  /// A description of this graph traversal.
  optional_t<Message> description;

  /// Values of relevant expressions at the start of the graph traversal that may change during graph traversal.
  optional_t<map_t<std::string, MultiformatMessageString>> initialState;

  /// Values of relevant expressions at the start of the graph traversal that remain constant for the graph traversal.
  optional_t<map_t<std::string, MultiformatMessageString>> immutableState;

  /// The sequences of edges traversed by this graph traversal.
  optional_t<std::vector<EdgeTraversal>> edgeTraversals;

  /// Key/value pairs that provide additional information about the graph traversal.
  optional_t<PropertyBag> properties;
};

/// Describes an HTTP request.
struct WebRequest {
  /// The index within the run.webRequests array of the request object associated with this result.
  optional_t<int32_t> index = -1;

  /// The request protocol. Example: 'http'.
  optional_t<std::string> protocol;

  /// The request version. Example: '1.1'.
  optional_t<std::string> version;

  /// The target of the request.
  optional_t<std::string> target;

  /// The HTTP method. Well-known values are 'GET', 'PUT', 'POST', 'DELETE', 'PATCH', 'HEAD', 'OPTIONS', 'TRACE', 'CONNECT'.
  optional_t<std::string> method;

  /// The request headers.
  optional_t<map_t<std::string, std::string>> headers;

  /// The request parameters.
  optional_t<map_t<std::string, std::string>> parameters;

  /// The body of the request.
  optional_t<ArtifactContent> body;

  /// Key/value pairs that provide additional information about the request.
  optional_t<PropertyBag> properties;
};

/// Describes the response to an HTTP request.
struct WebResponse {
  /// The index within the run.webResponses array of the response object associated with this result.
  optional_t<int32_t> index = -1;

  /// The response protocol. Example: 'http'.
  optional_t<std::string> protocol;

  /// The response version. Example: '1.1'.
  optional_t<std::string> version;

  /// The response status code. Example: 451.
  optional_t<int32_t> status_code;

  /// The response reason. Example: 'Not found'.
  optional_t<std::string> reasonPhrase;

  /// The response headers.
  optional_t<map_t<std::string, std::string>> headers;

  /// The body of the response.
  optional_t<ArtifactContent> body;

  /// Specifies whether a response was received from the server.
  optional_t<bool> noResponseReceived = false;

  /// Key/value pairs that provide additional information about the response.
  optional_t<PropertyBag> properties;
};

/// A function call within a stack trace.
struct StackFrame {
  /// The location to which this stack frame refers.
  Location location;

  /// The name of the module that contains the code of this stack frame.
  std::string module;

  /// The thread identifier of the stack frame.
  int32_t threadId = 0;

  /// The parameters of the call that is executing.
  std::vector<std::string> parameters;

  /// Key/value pairs that provide additional information about the stack frame.
  PropertyBag properties;
};

/// A call stack that is relevant to a result.
struct Stack {
  /// A message relevant to this call stack.
  optional_t<Message> message;

  /// An array of stack frames that represents a sequence of calls, rendered in reverse chronological order, that comprise the call stack.
  std::vector<StackFrame> frames;

  /// Key/value pairs that provide additional information about the stack.
  optional_t<PropertyBag> properties;
};

/// Describes a runtime exception encountered during the execution of an analysis tool.
struct Exception {
  /// A string that identifies the kind of exception, for example, the fully qualified type name of an object that was thrown, or the symbolic name of a signal.
  optional_t<std::string> kind;

  /// A message that describes the exception.
  optional_t<std::string> message;

  /// The sequence of function calls leading to the exception.
  optional_t<Stack> stack;

  /// An array of exception objects each of which is considered a cause of this exception.
  optional_t<std::vector<Exception>> innerExceptions;

  /// Key/value pairs that provide additional information about the exception.
  optional_t<PropertyBag> properties;
};

/// Information about how to locate a relevant reporting descriptor.
struct ReportingDescriptorReference {
  /// The id of the descriptor.
  /// @warning Any of this, `index`, or `guid` is required.
  optional_t<std::string> id;

  /// The index into an array of descriptors.
  /// @warning Any of this, `id`, or `guid` is required.
  optional_t<int32_t> index = -1;

  /// A guid that uniquely identifies the descriptor.
  /// @warning Any of this, `id`, or `index` is required.
  optional_t<std::string> guid;

  /// A reference used to locate the toolComponent associated with the descriptor.
  optional_t<ToolComponentReference> toolComponent;

  /// Key/value pairs that provide additional information about the reporting descriptor reference.
  optional_t<PropertyBag> properties;
};

/// A location visited by an analysis tool while simulating or monitoring the execution of a program.
struct ThreadFlowLocation {
  /// The index within the run threadFlowLocations array.
  optional_t<int32_t> index = -1;

  /// The code location.
  optional_t<Location> location;

  /// The call stack leading to this location.
  optional_t<Stack> stack;

  /// A set of distinct strings that categorize the thread flow location. Well-known kinds include 'acquire', 'release', 'enter', 'exit', 'call', 'return', 'branch', etc.
  optional_t<std::vector<std::string>> kinds;

  /// An array of references to rule or taxonomy reporting descriptors that are applicable to the thread flow location.
  optional_t<std::vector<ReportingDescriptorReference>> taxa;

  /// The name of the module that contains the code that is executing.
  optional_t<std::string> module;

  /// A dictionary, each of whose keys specifies a variable or expression, the associated value of which represents the variable or expression value.
  optional_t<map_t<std::string, MultiformatMessageString>> state;

  /// An integer representing a containment hierarchy within the thread flow.
  optional_t<int32_t> nestingLevel;

  /// An integer representing the temporal order in which execution reached this location.
  optional_t<int32_t> executionOrder = -1;

  /// The Coordinated Universal Time (UTC) date and time at which this location was executed.
  optional_t<std::string> execution_time_utc;

  /// Specifies the importance of this location in understanding the code flow. "essential", "important", "unimportant".
  optional_t<Importance> importance = Importance::important;

  /// A web request associated with this thread flow location.
  optional_t<WebRequest> webRequest;

  /// A web response associated with this thread flow location.
  optional_t<WebResponse> webResponse;

  /// Key/value pairs that provide additional information about the threadflow location.
  optional_t<PropertyBag> properties;
};

/// Describes a sequence of code locations that specify a path through a single thread of execution.
struct ThreadFlow {
  /// A string that uniquely identifies the threadFlow within the codeFlow in which it occurs.
  optional_t<std::string> id;

  /// A message relevant to the thread flow.
  optional_t<Message> message;

  /// Values of relevant expressions at the start of the thread flow that may change during thread flow execution.
  optional_t<map_t<std::string, MultiformatMessageString>> initialState;

  /// Values of relevant expressions at the start of the thread flow that remain constant.
  optional_t<map_t<std::string, MultiformatMessageString>> immutableState;

  /// A temporally ordered array of 'threadFlowLocation' objects.
  std::vector<ThreadFlowLocation> locations;

  /// Key/value pairs that provide additional information about the thread flow.
  optional_t<PropertyBag> properties;
};

/// A set of threadFlows which together describe a pattern of code execution relevant to detecting a result.
struct CodeFlow {
  /// A message relevant to the code flow.
  optional_t<Message> message;

  /// An array of one or more unique threadFlow objects.
  std::vector<ThreadFlow> threadFlows;

  /// Key/value pairs that provide additional information about the code flow.
  optional_t<PropertyBag> properties;
};

/// A suppression that is relevant to a result.
struct Suppression {
  enum class Kind : uint8_t {
    inSource,
    external,
  };

  enum class Status : uint8_t {
    accepted,
    underReview,
    rejected,
  };

  /// A stable, unique identifier for the suppression in the form of a GUID.
  optional_t<std::string> guid;

  /// A string that indicates where the suppression is persisted. "inSource" or "external".
  Kind kind = Kind::inSource;

  /// A string that indicates the review status of the suppression. "accepted", "underReview", or "rejected".
  optional_t<Status> status;

  /// A string representing the justification for the suppression.
  optional_t<std::string> justification;

  /// Identifies the location associated with the suppression.
  optional_t<Location> location;

  /// Key/value pairs that provide additional information about the suppression.
  optional_t<PropertyBag> properties;
};

/// Contains information about how and when a result was detected.
struct ResultProvenance {
  /// The Coordinated Universal Time (UTC) date and time at which the result was first detected.
  optional_t<std::string> firstDetectionTimeUtc;

  /// The Coordinated Universal Time (UTC) date and time at which the result was most recently detected.
  optional_t<std::string> lastDetectionTimeUtc;

  /// A GUID-valued string equal to the automationDetails.guid property of the run in which the result was first detected.
  optional_t<std::string> firstDetectionRunGuid;

  /// A GUID-valued string equal to the automationDetails.guid property of the run in which the result was most recently detected.
  optional_t<std::string> lastDetectionRunGuid;

  /// The index within the run.invocations array of the invocation object which describes the tool invocation that detected the result.
  optional_t<int32_t> invocationIndex = -1;

  /// An array of physicalLocation objects which specify the portions of an analysis tool's output that a converter transformed into the result.
  optional_t<std::vector<PhysicalLocation>> conversionSources;

  /// Key/value pairs that provide additional information about the result.
  optional_t<PropertyBag> properties;
};

/// Information about a rule or notification that can be configured at runtime.
struct ReportingConfiguration {
  /// Specifies whether the report may be produced during the scan.
  optional_t<bool> enabled = true;

  /// Specifies the failure level for the report. "none", "note", "warning", or "error".
  optional_t<Level> level = Level::warning;

  /// Specifies the relative priority of the report. Used for analysis output only.
  optional_t<double> rank = -1;

  /// Contains configuration information specific to a report.
  optional_t<PropertyBag> parameters;

  /// Key/value pairs that provide additional information about the reporting configuration.
  optional_t<PropertyBag> properties;
};

/// Information about the relation of one reporting descriptor to another.
struct ReportingDescriptorRelationship {
  /// A reference to the related reporting descriptor.
  ReportingDescriptorReference target;

  /// A set of distinct strings that categorize the relationship. Well-known kinds include 'canPrecede', 'canFollow', 'willPrecede', 'willFollow', 'superset', 'subset', 'equal', 'disjoint', 'relevant', and 'incomparable'.
  optional_t<std::vector<std::string>> kinds = std::vector<std::string>{ "relevant" };

  /// A description of the reporting descriptor relationship.
  optional_t<Message> description;

  /// Key/value pairs that provide additional information about the reporting descriptor reference.
  optional_t<PropertyBag> properties;
};

/// Metadata that describes a specific report produced by the tool.
struct ReportingDescriptor {
  /// A stable, opaque identifier for the report.
  std::string id;

  /// An array of stable, opaque identifiers by which this report was known in some previous version of the analysis tool.
  optional_t<std::vector<std::string>> deprecatedIds;

  /// A unique identifier for the reporting descriptor in the form of a GUID.
  optional_t<std::string> guid;

  /// An array of unique identifies in the form of a GUID by which this report was known in some previous version of the analysis tool.
  optional_t<std::vector<std::string>> deprecatedGuids;

  /// A report identifier that is understandable to an end user.
  optional_t<std::string> name;

  /// An array of readable identifiers by which this report was known in some previous version of the analysis tool.
  optional_t<std::vector<std::string>> deprecatedNames;

  /// A concise description of the report. Should be a single sentence.
  optional_t<MultiformatMessageString> shortDescription;

  /// A description of the report. Should provide details sufficient to enable resolution of any problem indicated by the result.
  optional_t<MultiformatMessageString> fullDescription;

  /// A set of name/value pairs with arbitrary names. Each value is a multiformatMessageString object.
  optional_t<map_t<std::string, MultiformatMessageString>> messageStrings;

  /// Default reporting configuration information.
  optional_t<ReportingConfiguration> defaultConfiguration;

  /// A URI where the primary documentation for the report can be found.
  optional_t<std::string> helpUri;

  /// Provides the primary documentation for the report, useful when there is no online documentation.
  optional_t<MultiformatMessageString> help;

  /// An array of objects that describe relationships between this reporting descriptor and others.
  optional_t<std::vector<ReportingDescriptorRelationship>> relationships;

  /// Key/value pairs that provide additional information about the report.
  optional_t<PropertyBag> properties;
};

/// A result produced by an analysis tool.
struct Result {
  enum class Kind : uint8_t {
    notApplicable,
    pass,
    fail,
    review,
    open,
    informational
  };

  /// The stable, unique identifier of the rule, if any, to which this result is relevant.
  optional_t<std::string> ruleId;

  /// The index within the tool component rules array of the rule object associated with this result.
  optional_t<int32_t> ruleIndex = -1;

  /// A reference used to locate the rule descriptor relevant to this result.
  optional_t<ReportingDescriptorReference> rule;

  /// A value that categorizes results by evaluation state.
  /// Valid values: "notApplicable", "pass", "fail", "review", "open", "informational"
  optional_t<Kind> kind = Kind::fail;

  /// A value specifying the severity level of the result.
  /// Valid values: "none", "note", "warning", "error"
  optional_t<Level> level = Level::warning;

  /// A message that describes the result. The first sentence of the message only will be displayed when visible space is limited.
  Message message;

  /// Identifies the artifact that the analysis tool was instructed to scan. This need not be the same as the artifact where the result actually occurred.
  optional_t<ArtifactLocation> analysisTarget;

  /// The set of locations where the result was detected. Specify only one location unless the problem indicated by the result can only be corrected by making a change at every specified location.
  optional_t<std::vector<Location>> locations;

  /// A stable, unique identifier for the result in the form of a GUID.
  optional_t<std::string> guid;

  /// A stable, unique identifier for the equivalence class of logically identical results to which this result belongs, in the form of a GUID.
  optional_t<std::string> correlationGuid;

  /// A positive integer specifying the number of times this logically unique result was observed in this run.
  optional_t<uint32_t> occurrenceCount;

  /// A set of strings that contribute to the stable, unique identity of the result.
  optional_t<map_t<std::string, std::string>> partialFingerprints;

  /// A set of strings each of which individually defines a stable, unique identity for the result.
  optional_t<map_t<std::string, std::string>> fingerprints;

  /// An array of 'stack' objects relevant to the result.
  optional_t<std::vector<Stack>> stacks;

  /// An array of 'codeFlow' objects relevant to the result.
  optional_t<std::vector<CodeFlow>> codeFlows;

  /// An array of zero or more unique graph objects associated with the result.
  optional_t<std::vector<Graph>> graphs;

  /// An array of one or more unique 'graphTraversal' objects.
  optional_t<std::vector<GraphTraversal>> graphTraversals;

  /// A set of locations relevant to this result.
  optional_t<std::vector<Location>> relatedLocations;

  /// A set of suppressions relevant to this result.
  optional_t<std::vector<Suppression>> suppressions;

  /// The state of a result relative to a baseline of a previous run.
  /// Valid values: "new", "unchanged", "updated", "absent"
  optional_t<std::string> baselineState;

  /// A number representing the priority or importance of the result.
  optional_t<double> rank = -1;

  /// A set of artifacts relevant to the result.
  optional_t<std::vector<Attachment>> attachments;

  /// An absolute URI at which the result can be viewed.
  optional_t<std::string> hostedViewerUri;

  /// The URIs of the work items associated with this result.
  optional_t<std::vector<std::string>> workItemUris;

  /// Information about how and when the result was detected.
  optional_t<ResultProvenance> provenance;

  /// An array of 'fix' objects, each of which represents a proposed fix to the problem indicated by the result.
  optional_t<std::vector<Fix>> fixes;

  /// An array of references to taxonomy reporting descriptors that are applicable to the result.
  optional_t<std::vector<ReportingDescriptorReference>> taxa;

  /// A web request associated with this result.
  optional_t<WebRequest> webRequest;

  /// A web response associated with this result.
  optional_t<WebResponse> webResponse;

  /// Key/value pairs that provide additional information about the result.
  optional_t<PropertyBag> properties;
};

/// Information that describes a run's identity and role within an engineering system process.
struct RunAutomationDetails {
  /// A description of the identity and role played within the engineering system by this object's containing run object.
  optional_t<Message> description;

  /// A hierarchical string that uniquely identifies this object's containing run object.
  optional_t<std::string> id;

  /// A stable, unique identifier for this object's containing run object in the form of a GUID.
  optional_t<std::string> guid;

  /// A stable, unique identifier for the equivalence class of runs to which this object's containing run object belongs in the form of a GUID.
  optional_t<std::string> correlationGuid;

  /// Key/value pairs that provide additional information about the run automation details.
  optional_t<PropertyBag> properties;
};

/// Defines locations of special significance to SARIF consumers.
struct SpecialLocations {
  /// Provides a suggestion to SARIF consumers to display file paths relative to the specified location.
  optional_t<ArtifactLocation> displayBase;

  /// Key/value pairs that provide additional information about the special locations.
  optional_t<PropertyBag> properties;
};

/// Provides additional metadata related to translation.
struct TranslationMetadata {
  /// The name associated with the translation metadata.
  std::string name;

  /// The full name associated with the translation metadata.
  optional_t<std::string> fullName;

  /// A brief description of the translation metadata.
  optional_t<MultiformatMessageString> shortDescription;

  /// A comprehensive description of the translation metadata.
  optional_t<MultiformatMessageString> fullDescription;

  /// The absolute URI from which the translation metadata can be downloaded.
  optional_t<std::string> downloadUri;

  /// The absolute URI from which information related to the translation metadata can be downloaded.
  optional_t<std::string> informationUri;

  /// Key/value pairs that provide additional information about the translation metadata.
  optional_t<PropertyBag> properties;
};

/// A component, such as a plug-in or the driver, of the analysis tool that was run.
struct ToolComponent {
  /// A unique identifier for the tool component in the form of a GUID.
  optional_t<std::string> guid;

  /// The name of the tool component.
  std::string name;

  /// The organization or company that produced the tool component.
  optional_t<std::string> organization;

  /// A product suite to which the tool component belongs.
  optional_t<std::string> product;

  /// A localizable string containing the name of the suite of products to which the tool component belongs.
  optional_t<std::string> productSuite;

  /// A brief description of the tool component.
  optional_t<MultiformatMessageString> shortDescription;

  /// A comprehensive description of the tool component.
  optional_t<MultiformatMessageString> fullDescription;

  /// The name of the tool component along with its version and other identifying information.
  optional_t<std::string> fullName;

  /// The tool component version in its native format.
  optional_t<std::string> version;

  /// The tool component version in Semantic Versioning 2.0 format.
  optional_t<std::string> semanticVersion;

  /// The binary version of the primary executable, as four integers separated by periods.
  optional_t<std::string> dottedQuadFileVersion;

  /// A string specifying the UTC release date and optional time.
  optional_t<std::string> releaseDateUtc;

  /// The absolute URI from which the tool component can be downloaded.
  optional_t<std::string> downloadUri;

  /// The absolute URI with information about this version of the tool component.
  optional_t<std::string> informationUri;

  /// A dictionary of message strings, keyed by resource identifier.
  optional_t<map_t<std::string, MultiformatMessageString>> globalMessageStrings;

  /// Reporting descriptors relevant to notifications.
  optional_t<std::vector<ReportingDescriptor>> notifications;

  /// Reporting descriptors relevant to analysis rules.
  optional_t<std::vector<ReportingDescriptor>> rules;

  /// Reporting descriptors relevant to taxonomy definitions.
  optional_t<std::vector<ReportingDescriptor>> taxa;

  /// Artifact locations associated with the tool component.
  optional_t<std::vector<ArtifactLocation>> locations;

  /// The language of emitted messages, defaulting to "en-US".
  optional_t<std::string> language = "en-US";

  /// The kinds of data contained in this object.
  optional_t<std::vector<LocalizedData>> contents = std::vector<LocalizedData>{ LocalizedData::localizedData, LocalizedData::nonLocalizedData };

  /// Specifies whether the object contains a complete definition of the component's data.
  optional_t<bool> isComprehensive = false;

  /// The semantic version of localized strings defined in this component.
  optional_t<std::string> localizedDataSemanticVersion;

  /// The minimum localizedDataSemanticVersion required in consumed translations.
  optional_t<std::string> minimumRequiredLocalizedDataSemanticVersion;

  /// The component strongly associated with this component.
  optional_t<ToolComponentReference> associatedComponent;

  /// Translation metadata (only for translation components).
  optional_t<TranslationMetadata> translationMetadata;

  /// Taxonomies supported by the tool component.
  optional_t<std::vector<ToolComponentReference>> supportedTaxonomies;

  /// Key/value pairs with additional information about the tool component.
  optional_t<PropertyBag> properties;
};

/// The analysis tool that was run.
struct Tool {
  /// The analysis tool that was run.
  ToolComponent driver;

  /// Tool extensions that contributed to or reconfigured the analysis tool that was run.
  optional_t<std::vector<ToolComponent>> extensions;

  /// Key/value pairs that provide additional information about the tool.
  optional_t<PropertyBag> properties;
};

/// Specifies the information necessary to retrieve a desired revision from a version control system.
struct VersionControlDetails {
  /// The absolute URI of the repository.
  std::string repositoryUri;

  /// A string that uniquely and permanently identifies the revision within the repository.
  optional_t<std::string> revisionId;

  /// The name of a branch containing the revision.
  optional_t<std::string> branch;

  /// A tag that has been applied to the revision.
  optional_t<std::string> revisionTag;

  /// A Coordinated Universal Time (UTC) date and time that can be used to synchronize an enlistment to the state of the repository at that time.
  optional_t<std::string> asOfTimeUtc;

  /// The location in the local file system to which the root of the repository was mapped at the time of the analysis.
  optional_t<ArtifactLocation> mappedTo;

  /// Key/value pairs that provide additional information about the version control details.
  optional_t<PropertyBag> properties;
};

/// Information about how a specific rule or notification was reconfigured at runtime.
struct ConfigurationOverride {
  /// Specifies how the rule or notification was configured during the scan.
  ReportingConfiguration configuration;

  /// A reference used to locate the descriptor whose configuration was overridden.
  ReportingDescriptorReference descriptor;

  /// Key/value pairs that provide additional information about the configuration override.
  optional_t<PropertyBag> properties;
};

/// Describes a condition relevant to the tool itself, as opposed to being relevant to a target being analyzed by the tool.
struct Notification {
  /// The locations relevant to this notification.
  optional_t<std::vector<Location>> locations;

  /// A message that describes the condition that was encountered.
  Message message;  // required

  /// A value specifying the severity level of the notification.
  /// Allowed values: "none", "note", "warning", "error".
  optional_t<Level> level = Level::warning;

  /// The thread identifier of the code that generated the notification.
  optional_t<int> threadId;

  /// The Coordinated Universal Time (UTC) date and time at which the analysis tool generated the notification.
  optional_t<std::string> timeUtc;

  /// The runtime exception, if any, relevant to this notification.
  optional_t<Exception> exception;

  /// A reference used to locate the descriptor relevant to this notification.
  optional_t<ReportingDescriptorReference> descriptor;

  /// A reference used to locate the rule descriptor associated with this notification.
  optional_t<ReportingDescriptorReference> associatedRule;

  /// Key/value pairs that provide additional information about the notification.
  optional_t<PropertyBag> properties;
};

/// The runtime environment of the analysis tool run.
struct Invocation {
  /// The command line used to invoke the tool.
  optional_t<std::string> commandLine;

  /// An array of strings, containing in order the command line arguments passed to the tool from the operating system.
  optional_t<std::vector<std::string>> arguments;

  /// The locations of any response files specified on the tool's command line.
  optional_t<std::vector<ArtifactLocation>> responseFiles;

  /// The Coordinated Universal Time (UTC) date and time at which the invocation started. See "Date/time properties" in the SARIF spec for the required format.
  optional_t<std::string> startTimeUtc;

  /// The Coordinated Universal Time (UTC) date and time at which the invocation ended. See "Date/time properties" in the SARIF spec for the required format.
  optional_t<std::string> endTimeUtc;

  /// The process exit code.
  optional_t<int> exitCode;

  /// An array of configurationOverride objects that describe rules related runtime overrides.
  optional_t<std::vector<ConfigurationOverride>> ruleConfigurationOverrides;

  /// An array of configurationOverride objects that describe notifications related runtime overrides.
  optional_t<std::vector<ConfigurationOverride>> notificationConfigurationOverrides;

  /// A list of runtime conditions detected by the tool during the analysis.
  optional_t<std::vector<Notification>> toolExecutionNotifications;

  /// A list of conditions detected by the tool that are relevant to the tool's configuration.
  optional_t<std::vector<Notification>> toolConfigurationNotifications;

  /// The reason for the process exit.
  optional_t<std::string> exitCodeDescription;

  /// The name of the signal that caused the process to exit.
  optional_t<std::string> exitSignalName;

  /// The numeric value of the signal that caused the process to exit.
  optional_t<int> exitSignalNumber;

  /// The reason given by the operating system that the process failed to start.
  optional_t<std::string> processStartFailureMessage;

  /// Specifies whether the tool's execution completed successfully.
  bool executionSuccessful = true;  // required

  /// The machine on which the invocation occurred.
  optional_t<std::string> machine;

  /// The account under which the invocation occurred.
  optional_t<std::string> account;

  /// The id of the process in which the invocation occurred.
  optional_t<int> processId;

  /// An absolute URI specifying the location of the executable that was invoked.
  optional_t<ArtifactLocation> executableLocation;

  /// The working directory for the invocation.
  optional_t<ArtifactLocation> workingDirectory;

  /// The environment variables associated with the analysis tool process, expressed as key/value pairs.
  optional_t<map_t<std::string, std::string>> environmentVariables;

  /// A file containing the standard input stream to the process that was invoked.
  optional_t<ArtifactLocation> stdin;

  /// A file containing the standard output stream from the process that was invoked.
  optional_t<ArtifactLocation> stdout;

  /// A file containing the standard error stream from the process that was invoked.
  optional_t<ArtifactLocation> stderr;

  /// A file containing the interleaved standard output and standard error stream from the process that was invoked.
  optional_t<ArtifactLocation> stdoutStderr;

  /// Key/value pairs that provide additional information about the invocation.
  optional_t<PropertyBag> properties;
};

/// Describes how a converter transformed the output of a static analysis tool
/// from the analysis tool's native output format into the SARIF format.
struct Conversion {
  /// A tool object that describes the converter.
  Tool tool;

  /// An invocation object that describes the invocation of the converter.
  optional_t<Invocation> invocation;

  /// The locations of the analysis tool's per-run log files.
  optional_t<std::vector<ArtifactLocation>> analysisToolLogFiles;

  /// Key/value pairs that provide additional information about the conversion.
  optional_t<PropertyBag> properties;
};

/// Contains information that enables a SARIF consumer to locate the external
/// property file that contains the value of an externalized property associated
/// with the run.
struct ExternalPropertyFileReference {
  /// The location of the external property file.
  /// @warning Either this or `guid` are required
  optional_t<ArtifactLocation> location;

  /// A stable, unique identifier for the external property file in the form of a GUID.
  /// @warning Either this or `location` are required
  optional_t<std::string> guid;

  /// A non-negative integer specifying the number of items contained in the external
  /// property file. Default is -1.
  optional_t<int> itemCount = -1;

  /// Key/value pairs that provide additional information about the external property file.
  optional_t<PropertyBag> properties;
};

/// Describes a single run of an analysis tool, and contains the reported output of that run.
struct Run {
  /// Information about the tool or tool pipeline that generated the results in this run. A run can only contain results produced by a single tool or tool pipeline. A run can aggregate results from multiple log files, as long as context around the tool run (tool command-line arguments and the like) is identical for all aggregated files.
  Tool tool;

  /// Describes the invocation of the analysis tool.
  optional_t<std::vector<Invocation>> invocations;

  /// A conversion object that describes how a converter transformed an analysis tool's native reporting format into the SARIF format.
  optional_t<Conversion> conversion;

  /// The language of the messages emitted into the log file during this run (expressed as an ISO 639-1 two-letter lowercase culture code) and an optional region (expressed as an ISO 3166-1 two-letter uppercase subculture code associated with a country or region). The casing is recommended but not required (in order for this data to conform to RFC5646).
  optional_t<std::string> language = "en-US";

  /// Specifies the revision in version control of the artifacts that were scanned.
  optional_t<std::vector<VersionControlDetails>> versionControlProvenance;

  /// The artifact location specified by each uriBaseId symbol on the machine where the tool originally ran.
  optional_t<map_t<std::string, ArtifactLocation>> originalUriBaseIds;

  /// An array of artifact objects relevant to the run.
  optional_t<std::vector<Artifact>> artifacts;

  /// An array of logical locations such as namespaces, types or functions.
  optional_t<std::vector<LogicalLocation>> logicalLocations;

  /// An array of zero or more unique graph objects associated with the run.
  optional_t<std::vector<Graph>> graphs;

  /// The set of results contained in an SARIF log. The results array can be omitted when a run is solely exporting rules metadata. It must be present (but may be empty) if a log file represents an actual scan.
  optional_t<std::vector<Result>> results;

  /// Automation details that describe this run.
  optional_t<RunAutomationDetails> automationDetails;

  /// Automation details that describe the aggregate of runs to which this run belongs.
  optional_t<std::vector<RunAutomationDetails>> runAggregates;

  /// The 'guid' property of a previous SARIF 'run' that comprises the baseline that was used to compute result 'baselineState' properties for the run.
  optional_t<std::string> baselineGuid;

  /// An array of strings used to replace sensitive information in a redaction-aware property.
  optional_t<std::vector<std::string>> redactionTokens;

  /// Specifies the default encoding for any artifact object that refers to a text file.
  optional_t<std::string> defaultEncoding;

  /// Specifies the default source language for any artifact object that refers to a text file that contains source code.
  optional_t<std::string> defaultSourceLanguage;

  /// An ordered list of character sequences that were treated as line breaks when computing region information for the run.
  /// { "\r\n", "\n" }
  optional_t<std::vector<std::string>> newlineSequences = std::vector<std::string>{ "\r\n", "\n" };

  /// Specifies the unit in which the tool measures columns.
  /// Valid values: "utf16CodeUnits", "unicodeCodePoints"
  optional_t<std::string> columnKind;

  /// References to external property files that should be inlined with the content of a root log file.
  optional_t<ExternalPropertyFileReference> externalPropertyFileReferences;

  /// An array of threadFlowLocation objects cached at run level.
  optional_t<std::vector<ThreadFlowLocation>> threadFlowLocations;

  /// An array of toolComponent objects relevant to a taxonomy in which results are categorized.
  optional_t<std::vector<ToolComponent>> taxonomies;

  /// Addresses associated with this run instance, if any.
  optional_t<std::vector<Address>> addresses;

  /// The set of available translations of the localized data provided by the tool.
  optional_t<std::vector<ToolComponent>> translations;

  /// Contains configurations that may potentially override both reportingDescriptor.defaultConfiguration (the tool's default severities) and invocation.configurationOverrides (severities established at run-time from the command line).
  optional_t<std::vector<ToolComponent>> policies;

  /// An array of request objects cached at run level.
  optional_t<std::vector<WebRequest>> webRequests;

  /// An array of response objects cached at run level.
  optional_t<std::vector<WebResponse>> webResponses;

  /// A specialLocations object that defines locations of special significance to SARIF consumers.
  optional_t<SpecialLocations> specialLocations;

  /// Key/value pairs that provide additional information about the run.
  optional_t<PropertyBag> properties;
};

}  // namespace cxxqa::sarif

/* JSON Enums
 ******************************************************************************/
namespace glz {

template <>
struct meta<cxxqa::sarif::Role> {
  using enum_type             = cxxqa::sarif::Role;
  static constexpr auto value = glz::enumerate(
    enum_type::analysisTarget, "analysisTarget", enum_type::attachment, "attachment", enum_type::responseFile, "responseFile", enum_type::resultFile, "resultFile", enum_type::standardStream, "standardStream", enum_type::tracedFile, "tracedFile", enum_type::unmodified, "unmodified", enum_type::modified, "modified", enum_type::added, "added", enum_type::deleted, "deleted", enum_type::renamed, "renamed", enum_type::uncontrolled, "uncontrolled", enum_type::driver, "driver", enum_type::extension, "extension", enum_type::translation, "translation", enum_type::taxonomy, "taxonomy", enum_type::policy, "policy", enum_type::referencedOnCommandLine, "referencedOnCommandLine", enum_type::memoryContents, "memoryContents", enum_type::directory, "directory", enum_type::userSpecifiedConfiguration, "userSpecifiedConfiguration", enum_type::toolSpecifiedConfiguration, "toolSpecifiedConfiguration", enum_type::debugOutputFile, "debugOutputFile"
  );
};

template <>
struct meta<cxxqa::sarif::Level> {
  using enum_type             = cxxqa::sarif::Level;
  static constexpr auto value = glz::enumerate(
    enum_type::none, "none", enum_type::note, "note", enum_type::warning, "warning", enum_type::error, "error"
  );
};

template <>
struct meta<cxxqa::sarif::BaselineState> {
  using enum_type             = cxxqa::sarif::BaselineState;
  static constexpr auto value = glz::enumerate(
    enum_type::new_, "new", enum_type::unchanged, "unchanged", enum_type::updated, "updated", enum_type::absent, "absent"
  );
};

template <>
struct meta<cxxqa::sarif::ColumnKind> {
  using enum_type             = cxxqa::sarif::ColumnKind;
  static constexpr auto value = glz::enumerate(
    enum_type::utf16CodeUnits, "utf16CodeUnits", enum_type::unicodeCodePoints, "unicodeCodePoints"
  );
};

template <>
struct meta<cxxqa::sarif::Importance> {
  using enum_type             = cxxqa::sarif::Importance;
  static constexpr auto value = glz::enumerate(
    enum_type::important, "important", enum_type::essential, "essential", enum_type::unimportant, "unimportant"
  );
};

template <>
struct meta<cxxqa::sarif::LocalizedData> {
  using enum_type             = cxxqa::sarif::LocalizedData;
  static constexpr auto value = glz::enumerate(
    enum_type::localizedData, "localizedData", enum_type::nonLocalizedData, "nonLocalizedData"
  );
};

template <>
struct meta<cxxqa::sarif::Suppression::Kind> {
  using enum_type             = cxxqa::sarif::Suppression::Kind;
  static constexpr auto value = glz::enumerate(
    enum_type::inSource, "inSource", enum_type::external, "external"
  );
};

template <>
struct meta<cxxqa::sarif::Suppression::Status> {
  using enum_type             = cxxqa::sarif::Suppression::Status;
  static constexpr auto value = glz::enumerate(
    enum_type::accepted, "accepted", enum_type::underReview, "underReview", enum_type::rejected, "rejected"
  );
};

template <>
struct meta<cxxqa::sarif::Result::Kind> {
  using enum_type             = cxxqa::sarif::Result::Kind;
  static constexpr auto value = glz::enumerate(
    enum_type::notApplicable, "notApplicable", enum_type::pass, "pass", enum_type::fail, "fail", enum_type::review, "review", enum_type::open, "open", enum_type::informational, "informational"
  );
};

}  // namespace glz

// NOLINTEND(readability-identifier-naming)
