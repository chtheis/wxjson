/////////////////////////////////////////////////////////////////////////////
// Name:        jsonval.h
// Purpose:     the wxJSONValue class: it holds a JSON value
// Author:      Luciano Cattani
// Created:     2007/09/15
// RCS-ID:      $Id: jsonval.h,v 1.4 2008/01/10 21:27:15 luccat Exp $
// Copyright:   (c) 2007 Luciano Cattani
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( _WX_JSONVAL_H )
#define _WX_JSONVAL_H

#ifdef __GNUG__
    #pragma interface "jsonval.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/hashmap.h>
    #include <wx/dynarray.h>
    #include <wx/arrstr.h>
#endif


#include "json_defs.h"

// forward declarantions
class WXDLLIMPEXP_JSON wxJSONValue;
class WXDLLIMPEXP_JSON wxJSONInternalMap;

WX_DECLARE_OBJARRAY( wxJSONValue, wxJSONInternalArray );


//! The type of the value held by the wxJSONValue class
enum wxJSONType {
    wxJSONTYPE_EMPTY = 0,  /*!< empty type is for uninitialized objects  */
    wxJSONTYPE_NULL,       /*!< the object contains a NULL value         */
    wxJSONTYPE_INT,        /*!< the object contains an integer           */
    wxJSONTYPE_UINT,       /*!< the object contains an unsigned integer  */
    wxJSONTYPE_DOUBLE,     /*!< the object contains a double             */
    wxJSONTYPE_STRING,     /*!< the object contains a wxString object    */
    wxJSONTYPE_CSTRING,    /*!< the object contains a static C-string    */
    wxJSONTYPE_BOOL,       /*!< the object contains a boolean            */
    wxJSONTYPE_ARRAY,      /*!< the object contains an array of values   */
    wxJSONTYPE_OBJECT      /*!< the object contains a map of keys/values */
};

// the comment position: every value only has one comment position
// althrough comments may be splitted into several lines
enum {
  wxJSONVALUE_COMMENT_DEFAULT = 0,
  wxJSONVALUE_COMMENT_BEFORE,
  wxJSONVALUE_COMMENT_AFTER,
  wxJSONVALUE_COMMENT_INLINE,
};

//! The actual value held by the wxJSONValue class (internal use)
/*!
 Note that this structure is a \b union. This means that the actual value
 depends on the \b type of the value. 
*/
union wxJSONValueHolder  {
    int           m_valInt;
    unsigned      m_valUInt;
    double        m_valDouble;
    const wxChar* m_valCString;
    bool          m_valBool;
    wxString*     m_valString;
    wxJSONInternalArray* m_valArray;
    wxJSONInternalMap*   m_valMap;
  };

class wxJSONReader;

class WXDLLIMPEXP_JSON wxJSONValue
{
  friend class wxJSONReader;

public:

  // ctors and dtor
  wxJSONValue();
  wxJSONValue( wxJSONType type );
  wxJSONValue( int i );
  wxJSONValue( unsigned int i );
  wxJSONValue( bool b  );
  wxJSONValue( double d );
  wxJSONValue( const wxChar* str );     // assume static ASCIIZ strings
  wxJSONValue( const wxString& str );
  wxJSONValue( const wxJSONValue& other );
  ~wxJSONValue();

  // get/set the value type
  wxJSONType  GetType() const;
  void SetType( wxJSONType type );
  bool IsEmpty() const;
  bool IsNull() const;
  bool IsInt() const;
  bool IsUInt() const;
  bool IsBool() const;
  bool IsDouble() const;
  bool IsString() const;
  bool IsCString() const;
  bool IsArray() const;
  bool IsObject() const;

  // get the value as ...
  int          AsInt() const;
  bool         AsBool() const;
  double       AsDouble() const;
  wxString     AsString() const;
  const wxChar* AsCString() const;
  unsigned int  AsUInt() const;
  const wxJSONInternalMap*   AsMap() const;
  const wxJSONInternalArray* AsArray() const;

  // get members names, size and other info
  bool      HasMember( unsigned index ) const;
  bool      HasMember( const wxString& key ) const;
  int       Size() const;
  wxArrayString  GetMemberNames() const;

  // appending items, resizing and deleting items
  wxJSONValue& Append( const wxJSONValue& value );
  wxJSONValue& Append( int i );
  wxJSONValue& Append( bool b );
  wxJSONValue& Append( unsigned int ui );
  wxJSONValue& Append( double d );
  wxJSONValue& Append( const wxChar* str );
  wxJSONValue& Append( const wxString& str );
  bool         Remove( int index );
  bool         Remove( const wxString& key );
  void         Clear();
  bool         Cat( const wxChar* str );
  bool         Cat( const wxString& str );

  // retrieve an item
  wxJSONValue& Item( unsigned index );
  wxJSONValue& Item( const wxString& key );
  wxJSONValue  ItemAt( unsigned index ) const;
  wxJSONValue  ItemAt( const wxString& key ) const;

  wxJSONValue& operator [] ( unsigned index );
  wxJSONValue& operator [] ( const wxString& key );

  wxJSONValue& operator = ( int i );
  wxJSONValue& operator = ( unsigned int ui );
  wxJSONValue& operator = ( bool b );
  wxJSONValue& operator = ( double d );
  wxJSONValue& operator = ( const wxChar* str );
  wxJSONValue& operator = ( const wxString& str );
  wxJSONValue& operator = ( const wxJSONValue& value );

  // get the value of a default value
  wxJSONValue  Get( const wxString& key, const wxJSONValue& defaultValue ) const;

  // comparison function
  bool         IsSameAs( const wxJSONValue& other ) const;

  // comment-related functions
  int      AddComment( const wxString& str, int position = wxJSONVALUE_COMMENT_DEFAULT );
  int      AddComment( const wxArrayString& comments, int position = wxJSONVALUE_COMMENT_DEFAULT );
  wxString GetComment( int idx = -1 ) const;
  int      GetCommentPos() const;
  int      GetCommentCount() const;
  void     ClearComments();
  const wxArrayString& GetCommentArray() const;

  // debugging functions
  wxString         GetInfo() const;
  static  wxString TypeToString( wxJSONType type ); 

protected:
  wxJSONValue*  Find( unsigned index ) const;
  wxJSONValue*  Find( const wxString& key ) const;
  void          DeleteObj();
  void          DeepCopy( const wxJSONValue& other );

private:
  //! The actual type of the value held by this object.
  wxJSONType        m_type;

  //! The JSON value held by this object.
  /*!
   This data member is an \b union of the various types defined by the
   JSON syntax. For some types (for example, wxJSONTYPE_NULL) the value is
   meaningless.
   The data member contains the actual value for primitive types and the
   pointer to the actual data structure for complex types (string, hashmap
   and array).
  */
  wxJSONValueHolder m_value;

  //! The position of the comment line(s), if any.
  /*!
   The data member contains one of the following constants:
   \li \c wxJSONVALUE_COMMENT_BEFORE
   \li \c wxJSONVALUE_COMMENT_AFTER
   \li \c wxJSONVALUE_COMMENT_INLINE
  */
  int               m_commentPos;

  //! The array of comment lines; may be empty.
  wxArrayString     m_comments;

  //! The line number when this value was read
  /*!
   This data member is used by the wxJSONReader class and it is
   used to store the line number in the text input stream where
   the value appeared
  */ 
  int               m_lineNo;

  // used for debugging purposes: only in debug builds.
#if defined( WXJSON_USE_VALUE_COUNTER )
  int         m_progr;
  static int  sm_progr;
#endif
};

WX_DECLARE_STRING_HASH_MAP( wxJSONValue, wxJSONInternalMap );


#endif			// not defined _WX_JSONVAL_H


