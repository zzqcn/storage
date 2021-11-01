语法演示
=========

行
----

原始行: 这是一个较长的
文本行.


解决方法1: 这是一个较长的文本行.

解决方法2: 这是一个较长的\
文本行.


段落
------

普通段落
^^^^^^^^^^

This is first paragraph.

This is second paragraph.

强制换行段落
^^^^^^^^^^^^^^

| Lend us a couple of bob till Thursday.
| I'm absolutely skint.
| But I'm expecting a postal order and I can pay you back
  as soon as it comes.
| Love, Ewan.


引用段落
^^^^^^^^^^

This is an ordinary paragraph, introducing a block quote.

    "It is my business to know things.  That is my trade."

    -- Sherlock Holmes

缩进文字块
^^^^^^^^^^^^

This is a typical paragraph.  An indented literal block follows.

::

    for a in [5,4,3,2,1]:   # this is program code, shown as-is
        print a
    print "it's..."
    # a literal block continues until the indentation ends

This text has returned to the indentation of the first paragraph,
is outside of the literal block, and is therefore treated as an
ordinary paragraph.

引用文字块
^^^^^^^^^^^^

John Doe wrote::

>> Great idea!
>
> Why didn't I think of that?

You just did!  ;-)

分隔线
--------

A transition marker is a horizontal line
of 4 or more repeated punctuation
characters.

------------

A transition should not begin or end a
section or document, nor should two
transitions be immediately adjacent.

行内标记
----------

XXX **加粗** XXX *斜体* XXX ``行内代码`` XXX

.. _target:

列表
-------

无序列表
^^^^^^^^^^^

- This is the first bullet list item.  The blank line above the
  first list item is required; blank lines between list items
  (such as below this paragraph) are optional.

- This is the first paragraph in the second item in the list.

  This is the second paragraph in the second item in the list.
  The blank line above this paragraph is required.  The left edge
  of this paragraph lines up with the paragraph above, both
  indented relative to the bullet.

  - This is a sublist.  The bullet lines up with the left edge of
    the text blocks above.  A sublist is a new list so requires a
    blank line above and below.

- This is the third item of the main list.

This paragraph is not part of the list.

有序列表
^^^^^^^^^^

1. Item 1 initial text.

   a) Item 1a.
   b) Item 1b.

2. a) Item 2a.
   b) Item 2b.

定义列表
^^^^^^^^^

term 1
    Definition 1.

term 2
    Definition 2, paragraph 1.

    Definition 2, paragraph 2.

term 3 : classifier
    Definition 3.

term 4 : classifier one : classifier two
    Definition 4.

\-term 5
    Without escaping, this would be an option list item.

字段列表
^^^^^^^^^^

:Date: 2001-08-16
:Version: 1
:Authors: - Me
          - Myself
          - I
:Indentation: Since the field marker may be quite long, the second
   and subsequent lines of the field body do not have to line up
   with the first line, but they must be indented relative to the
   field name marker, and they must line up with each other.
:Parameter i: integer

选项列表
^^^^^^^^^

-a         Output all.
-b         Output both (this description is
           quite long).
-c arg     Output just arg.
--long     Output all day long.

-p         This option has two paragraphs in the description.
           This is the first.

           This is the second.  Blank lines may be omitted between
           options (as above) or left in (as here and below).

--very-long-option  A VMS-style option.  Note the adjustment for
                    the required two spaces.

--an-even-longer-option
           The description can also start on the next line.

-2, --two  This option has two variants.

-f FILE, --file=FILE  These two options are synonyms; both have
                      arguments.

/V         A VMS/DOS-style option.


表格
------

简单表格
^^^^^^^^^^

=====  =====  ======
   Inputs     Output
------------  ------
  A      B    A or B
=====  =====  ======
False  False  False
True   False  True
False  True   True
True   True   True
=====  =====  ======

绘制表格
^^^^^^^^^^

+------------------------+------------+----------+----------+
| Header row, column 1   | Header 2   | Header 3 | Header 4 |
| (header rows optional) |            |          |          |
+========================+============+==========+==========+
| body row 1, column 1   | column 2   | column 3 | column 4 |
+------------------------+------------+----------+----------+
| body row 2             | Cells may span columns.          |
+------------------------+------------+---------------------+
| body row 3             | Cells may  | - Table cells       |
+------------------------+ span rows. | - contain           |
| body row 4             |            | - body elements.    |
+------------------------+------------+---------------------+


超链接
----------

外部超链接
^^^^^^^^^^^^

visit `baidu <http://www.baidu.com>`_

visit `baidu`_

.. _baidu: http://www.baidu.com


内部超链接
^^^^^^^^^^^^

Clicking on this internal hyperlink will take us to the target_ .

图片
------

image
^^^^^^^

.. image:: img/vscode.png
   :scale: 10%
   :alt: vscode
   :align: center

figure
^^^^^^^^^

.. figure:: img/vscode.png
   :scale: 30 %
   :alt: vscode

   VSCode Logo

代码
-------

code
^^^^^^

.. code:: c
    :number-lines:

    void foo()
    {
        int i;

        for(i=0; i<10; i++)
        printf("i: %d\n", a);
    }


code-block
^^^^^^^^^^^^

.. code-block:: c
    :linenos:
    :emphasize-lines: 3,6

    void foo()
    {
        int i;

        for(i=0; i<10; i++)
        printf("i: %d\n", a);
    }


提示区块
----------

.. note:: this is a note.

.. warning:: this is a warning.

.. error:: this is an error.


注释
------

.. This is a comment.

..
   This whole indented block
   is a comment.

   Still in the comment.

