=============================
SDKDB Format Reference Manual
=============================

.. contents::
   :local:
   :depth: 4

.. _sdkdb_abstract:

Abstract
========

SDKDB is a database containing all the interfaces from a SDK, both public
and private. The primary usage of SDKDB is API/SPI checking for 3rd party
apps in the AppStore. It can also be used to perform SDK API regression
checking, availability info verification, etc.

Currently, SDKDB contains information about:

- Symbols: exported symbols from the dynamic libraries. They are interfaces
  that can be linked and called from other binaries. They are mostly coming from
  C function interfaces or globals.

- Objective-C Classes: information of Objective-C classes, including its
  name, super class, all methods and conformed protocols.

- Objective-C Category: information of Objective-C category, including its
  name, base class, all methods and conformed protocols.

- Objective-C Protocol: information of Objective-C protocol, including its
  name, all methods and conformed protocols.


.. _sdkdb_format:

Format
======

Currently, SDKDB is in JSON format. It can be easily process by any JSON
parser. SDKDB is a JSON object on the top level. It is a map from
:ref:`target <sdkdb_target>` to an array of :ref:`APIs <sdkdb_api>` where each of them
represents all the exported interfaces from a binary. There are few variants
of SDKDB format and all of them are heavily dependent on :ref:`API <sdkdb_api>`
data structure.

Please refer to :ref:`JSON Data Structure <sdkdb_json>` for detailed information
about all information available in the SDKDB and how to interpret them.
Please note that all the fields with empty array, empty string, default enum
value or false are omitted from SDKDB to save space.

.. _sdkdb_SDKDB:

SDKDB
-----

Here is a simple example of SDKDB:

.. code-block:: python

 {
   "arm64-apple-ios12": [
     {
       "binaryInfo": {
         "compatibilityVersion": "1",
         "currentVersion": "1",
         "installName": "/usr/lib/libtest.1.dylib",
         "reexportedLibraries": [
           "/usr/lib/libfoo.dylib"
         ],
         "twoLevelNamespace": true,
         "type": "dylib"
       },
       "globals": [
         {
           "access": "public",
           "file": "test.h",
           "linkage": "exported",
           "name": "_bar"
         }
       ],
       "interfaces": [
         {
           "access": "public",
           "classMethods": [
             {
               "name": "new"
             }
           ],
           "file": "test.h",
           "instanceMethods": [
             {
               "name": "foo:"
             },
             {
               "access": "public",
               "introduced": "9.0",
               "name": "init"
             }
           ],
           "linkage": "exported",
           "name": "Foo",
           "super": "NSObject"
         }
       ]
     }
   ]
 }

There is also a public-only variation of SDKDB which only stores APIs that are
public. In this case, :ref:`access <sdkdb_access>` field is omitted because all of
them are public. To distinguish from the normal SDKDB, it also has a boolean
field to indicate it is public. The public only SDKDB for the dylib above looks
like:

.. code-block:: python

 {
   "arm64-apple-ios12": [
     {
       "binaryInfo": {
         "compatibilityVersion": "1",
         "currentVersion": "1",
         "installName": "/usr/lib/libtest.1.dylib",
         "reexportedLibraries": [
           "/usr/lib/libfoo.dylib"
         ],
         "twoLevelNamespace": true,
         "type": "dylib"
       },
       "globals": [
         {
           "file": "test.h",
           "linkage": "exported",
           "name": "_bar"
         }
       ],
       "interfaces": [
         {
           "file": "test.h",
           "instanceMethods": [
             {
               "introduced": "9.0",
               "name": "init"
             }
           ],
           "linkage": "exported",
           "name": "Foo",
           "super": "NSObject"
         }
       ]
     }
   ],
   "public": true
 }

.. _sdkdb_partialSDKDB:

Partial SDKDB
-------------

Partial SDKDBs are the intermediate representation for SDK information before
assembling into full SDKDB. It contains information of RuntimeRoot,
SDKContentRoot and PublicSDKContentRoot seperately, and each of them is
represented by an array of :ref:`APIs <sdkdb_api>`. There is also a version
key to represent the partial SDKDB version. The current supported version
is "1.0". If the interface scanning failed in anyway during partial SDKDB
generation, "error" bit will be set. "projectName" corresponding to
"RC_ProjectName" XBS environment when partial SDKDB is generated.

Here is an example:

.. code-block:: python

  {
     "PublicSDKContentRoot": [],
     "RuntimeRoot": [
       {
          "//": "SomeAPI"
       }
     ],
     "SDKContentRoot" : [
       {
          "//": "SomeAPI"
       }
     ],
     "error": true,
     "projectName": "MyProject",
     "version": "1.0"
  }

.. _sdkdb_json:

JSON Data Structure
===================

.. _sdkdb_api:

API
---
API is an object that includes following key values:

- :ref:`target <sdkdb_target>`: supported target, it is omitted when emitting SDKDB.

- :ref:`globals <sdkdb_global>`: an array of globals

- :ref:`interfaces <sdkdb_interface>`: an array of interfaces

- :ref:`protocols <sdkdb_protocol>`: an arrary of procotols

- :ref:`potentially defined selectors <sdkdb_potentiallyDefinedSelectors>`:
  an array of potentially defined selectors in the binary

- :ref:`binaryInfo <sdkdb_binary>`: an object representing binary information

.. _sdkdb_target:

Target
------

Target is a LLVM triple representing the target architecture, platform, os
and environment. Some common targets on Apple platforms are:

- arm64-apple-ios12
- x86_64-apple-macosx10.14
- i386-apple-ios9-simulator

.. _sdkdb_global:

Globals
-------

Globals represent all the symbols from the given API representing a binary.
It is a json array where each of its element represents an exported symbol.
Symbols in the array has following fields:

- name: name of the symbol
- :ref:`availability <sdkdb_availability>`
- :ref:`access <sdkdb_access>`
- :ref:`source location <sdkdb_source_location>`
- :ref:`linkage <sdkdb_linkage>`
- weakDefined: true if the symbol is weak defined
- weakReferenced: true if the symbol is weak referenced
- threadLocalValue: true if the symbols is thread local value
- kind: function or variable, if not specified, then unknown

.. _sdkdb_interface:

Interface
---------

Interface represents ObjC class. It has following fields:

- name: name of the objc class
- :ref:`availability <sdkdb_availability>`
- :ref:`access <sdkdb_access>`
- :ref:`source location <sdkdb_source_location>`
- super: super class name
- :ref:`linkage <sdkdb_linkage>`
- hasException: whether objc class has exception
- procotols: list of conformed protocol name
- classMethods: list of class :ref:`methods <sdkdb_method>`
- instanceMethods: list of instance :ref:`methods <sdkdb_method>`
- properties: list of :ref:`properties <sdkdb_property>`

.. _sdkdb_category:

Category
--------

Category has following fields:

- name: name of the objc category
- :ref:`availability <sdkdb_availability>`
- :ref:`access <sdkdb_access>`
- :ref:`source location <sdkdb_source_location>`
- interface: the objc class which category extends
- procotols: list of conformed protocol name
- classMethods: list of class :ref:`methods <sdkdb_method>`
- instanceMethods: list of instance :ref:`methods <sdkdb_method>`
- properties: list of :ref:`properties <sdkdb_property>`

.. _sdkdb_protocol:

Protocol
--------

Protocol has following fields:

- name: name of the objc protocol
- :ref:`availability <sdkdb_availability>`
- :ref:`access <sdkdb_access>`
- :ref:`source location <sdkdb_source_location>`
- procotols: list of conformed protocol name
- classMethods: list of class :ref:`methods <sdkdb_method>`
- instanceMethods: list of instance :ref:`methods <sdkdb_method>`
- properties: list of :ref:`properties <sdkdb_property>`

.. _sdkdb_potentiallyDefinedSelectors:

Potentially Defined Selectors
-----------------------------

An array of strings which represents the potentially defined selectors which
can be used to dynamically constructing objc metadata during runtime for swift
generics.

.. _sdkdb_binary:

BinaryInfo
----------

BinaryInfo has following fields:

- type: dylib, stub, bundle or invalid
- currentVersion: current version for the dylib
- compatibilityVersion: compatibility version for the dylib
- objcConstraint: Objective-C contraints
- installName: install name for the binary
- uuid: optional uuid for the binary
- parentUmbrella: umbrella framework if exists
- swiftABI: swiftABI if binary uses swift
- twoLevelNamespace: if binary uses two level namespace
- appExtensionSafe: if binary is app extension safe
- allowableClients: a list of allowable clients
- reexportedLibraries: a list of re-exported frameworks/dylibs

.. _sdkdb_method:

Method
------

Method has following field:

- name: name of the objc method
- :ref:`availability <sdkdb_availability>`
- :ref:`access <sdkdb_access>`
- :ref:`source location <sdkdb_source_location>`
- optional: if the method is optional
- dynamic: if the method is dynamic

.. _sdkdb_property:

Property
--------

Property has following field:

- name: name of the objc property
- :ref:`availability <sdkdb_availability>`
- :ref:`access <sdkdb_access>`
- :ref:`source location <sdkdb_source_location>`
- attr: objc attributes. It can has one or more of following values: readonly,
  dynamic, class
- getter: getter name
- setter: setter name

.. _sdkdb_availability:

Availability
------------

Availability encodes the source level availability information for the API.
It can have following 4 fields:

- introduced: the version api is introduced, omitted if not set.
- obsoleted: the version api is obsoleted, omitted if not set.
- unavailable: if the api is marked as unavailable, omitted if available.
- SPIAvailable: if the availability is declared with SPI_AVAILABLE, omitted if not.

.. _sdkdb_source_location:

Source Location
---------------

Source location can have following three fields to represent where the API
is declared in the SDK.

- file: The source file/header
- line: The line in the file that it is declared
- col: The column in the line that it is declared

Currently, SDKDB only records "file" and the other two fields are omitted.

.. _sdkdb_access:

Access
------

This represents the access level for the API:

- public: Available in public header, ships in public SDK
- private: Available in private header, not shipped in public SDK
- project: Only available in project header, not shipped in any SDKs
- unknown: Unknown access

.. _sdkdb_linkage:

Linkage
-------

Linkage can following value:

- exported: the interface is exported from the binary. Other binary can link
  can call such interfaces.
- re-exported: the interface is re-exported from another binary. Other binary
  can call such interfaces.
- internal: the interface is internal to the binary. Other binary cannot link
  or call such interfaces.
- external: the interface referenced by the binary which is available in other
  dylib/framework.

SDKDB only stores exported symbols so linkage in SDKDB can only be exported
or re-exported.
