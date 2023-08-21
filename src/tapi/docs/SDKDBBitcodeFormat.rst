=====================================
SDKDB Bitcode Format Reference Manual
=====================================

.. contents::
   :local:
   :depth: 4

.. _sdkdb_bitcode_abstract:

Abstract
========

SDKDB bitcode is used to serialize out SDKDB in bitstream format in order to:

- be more space efficient than the JSON representation
- can be parsed much faster than JSON representation
- can lazily parse the part of the SDKDB that is needed for the current job


Format
======

Magic Number
------------

The SDKDB magic number is **'S' 'D' 'K' 0xDB**.

Control Block
-------------

The first block of the bitcode storage is control block, which contains
metadata for the current version and other basic information about the SDKDB
format.

Metadata has following fields:

- version: version is encoded as Fixed<16> MajorVersion Fixed<16> MinorVersion.
- flags: Fixed<16> to indicate SDKDBBuilderOptions. The lowest bit indicates a public-only SDKDB.
- buildVersion: a Blob encoding the build version of the SDKDB.

The current version is 1.0.

Control block also encodes projects that failed to generate interface for.
They are encoded as blob with record of PROJECT_WITH_ERROR.

Identification Block
--------------------

The identification block stores all the strings referenced by the SDKDB.
There should only be one identification block in the file.

The block is encoded with a StringTable. Later blocks can reference a string
in the stringtable using an index into the block.


.. _sdkdb_bitcode_string_encoding:

StringTableEncoding
^^^^^^^^^^^^^^^^^^^

Most of the strings in SDKDB are stored in the StringTable and indexed via
offset. The encoding of string is to use two fields:

- Offset: VBR<8> indicates the offset in StringTable.
- Size: VBR<6> indicates the size of the string.


.. _sdkdb_bitcode_sdkdb_block:

SDKDB Block
-----------

SDKDB block represents the SDKDB information for one target triple. The first
record in the SDKDB block must be TARGET_TRIPLE, which indicates the target
triple for this SDKDB block.

TARGET_TRIPLE is encoded as a Blob.

It is then followed by arbitrary number of :ref:`API Blocks <sdkdb_bitcode_api_block>`.


.. _sdkdb_bitcode_api_block:

API Block
---------

API Block encodes all the information in API Class/Object. It represents the 
APIs of the binary in the system. It has the following records:

- INSTALL_NAME: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`. install name of the binary.
- UUID: Blob data. UUID of the binary.
- FILE_TYPE: Fixed<4>, enum type for the binary file type.
- REEXPORTED: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`. re-exported libaries.
  This appears multiple times or is omitted if the binary doesn't re-export.
- PARENT_UMBRELLA: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
  Name of the umbrella framework.
- DYLIB_VERISON: [Fix<32>, Fix<32>]. CurrentVersion and CompatilityVersion.
- FLAGS: [Fix<1>, Fix<1>], TwoLevelNamespace and AppExtensionSafe.
- SWIFT_VERSION: Fix<8>. Swift Runtime ABI version.
- POTENTIALLY_DEFINED_SELECTOR: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.

Other the records, API Block can also have sub-blocks of following types:

- :ref:`Global Block <sdkdb_bitcode_global_block>`.
- :ref:`ObjC Class Block <sdkdb_bitcode_objc_class_block>`.
- :ref:`ObjC Category Block <sdkdb_bitcode_objc_category_block>`.
- :ref:`ObjC Protocol Block <sdkdb_bitcode_objc_protocol_block>`.


.. _sdkdb_bitcode_global_block:

Global Block
------------

Encode exported global.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.
- kind: Fixed<2>.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

.. _sdkdb_bitcode_objc_class_block:

ObjC Class Block
----------------

Encode one ObjC class.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.
- superClassName: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.

PROTOCOL record:

- protocol: can appear multiple times. :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

It can have optional sub-blocks:

- :ref:`ObjC Method Block <sdkdb_bitcode_objc_method_block>`.
- :ref:`ObjC Property Block <sdkdb_bitcode_objc_property_block>`.
- :ref:`ObjC iVar Block <sdkdb_bitcode_objc_ivar_block>`.


.. _sdkdb_bitcode_objc_category_block:

ObjC Category Block
-------------------

Encode one ObjC category.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.
- interfaceName: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`..

PROTOCOL record:

- protocol: can appear multiple times. :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

It can have optional sub-blocks:

- :ref:`ObjC Method Block <sdkdb_bitcode_objc_method_block>`.
- :ref:`ObjC Property Block <sdkdb_bitcode_objc_property_block>`.
- :ref:`ObjC iVar Block <sdkdb_bitcode_objc_ivar_block>`.


.. _sdkdb_bitcode_objc_protocol_block:

ObjC Protocol Block
-------------------

Encode one ObjC protocol.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.

PROTOCOL record:

- protocol: can appear multiple times. :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

It can have optional sub-blocks:

- :ref:`ObjC Method Block <sdkdb_bitcode_objc_method_block>`.
- :ref:`ObjC Property Block <sdkdb_bitcode_objc_property_block>`.
- :ref:`ObjC iVar Block <sdkdb_bitcode_objc_ivar_block>`.

.. _sdkdb_bitcode_objc_method_block:

ObjC Method Block
-----------------

Encode one ObjC method.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.
- isInstanceMethod: Fixed<1>.
- isOptional: Fixed<1>.
- isDynamic: Fixed<1>.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

.. _sdkdb_bitcode_objc_property_block:

ObjC Property Block
-------------------

Encode one ObjC property.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.
- attributes: Fixed<3>.
- isOptional: Fixed<1>.
- getter: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- setter: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

.. _sdkdb_bitcode_objc_ivar_block:

ObjC iVar Block
---------------

Encode one ObjC ivar.

INFO record:

- name: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.
- access: Fixed<2>. If this is public or private.
- linkage: Fixed<3>.
- flags: Fixed<3>.

It can have following optional record types:

- :ref:`Availability Record <sdkdb_bitcode_availability_record>`.
- :ref:`Filename Record <sdkdb_bitcode_filename_record>`.
- :ref:`Location Record <sdkdb_bitcode_location_record>`.

.. _sdkdb_bitcode_library_table_block:

Library Table Block
-------------------

This is on-disk hash table to lookup the corresponding API block entry offset
from the path of the dylib.

The first record in the block is a blob with text encoding for the target
triple, followed by the second record which is blob represents the on-disk
hash table.

Hash table is configured with:

- key: StringRef (pointer to identificaion block, size)
- data: offset (offset of the correponding API block)

.. _sdkdb_bitcode_availability_record:

Availability Record
-------------------

Encode availability info for the API. AVAILABILITY record:

- introduced: VBR<6>.
- obsoleted: VBR<6>.
- unavailable: Fixed<1>.
- isSPIAvailable: Fixed<1>.


.. _sdkdb_bitcode_filename_record:

Filename Record
---------------

Encode source file of the API.

- file: :ref:`StringTableEncoding <sdkdb_bitcode_string_encoding>`.


.. _sdkdb_bitcode_location_record:

Location Record
---------------

LOCATION record:

- line: VBR<6>.
- column: VBR<6>.

