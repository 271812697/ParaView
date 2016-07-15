/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

   Copyright (c) 2013 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#ifndef pqOpacityTableModel_h
#define pqOpacityTableModel_h

#include <QAbstractTableModel>

class pqColorOpacityEditorWidget;

// QAbstractTableModel subclass for keeping track of the opacity transfer
// function control points.
// First column is control point scalar value and the second is the opacity.
class pqOpacityTableModel : public QAbstractTableModel
{
Q_OBJECT
  typedef QAbstractTableModel Superclass;

public:
  pqOpacityTableModel(pqColorOpacityEditorWidget * widget, QObject* parentObject = 0);

  virtual ~pqOpacityTableModel() {}

  /// All columns are editable.
  virtual Qt::ItemFlags flags(const QModelIndex &idx) const;

  virtual bool setData(const QModelIndex &idx, const QVariant &value, int role=Qt::EditRole);

  virtual int rowCount(const QModelIndex & parent=QModelIndex()) const;

  virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex& idx, int role=Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  virtual void refresh();

private:
  Q_DISABLE_COPY(pqOpacityTableModel)

  pqColorOpacityEditorWidget * Widget;

  int NumberOfRowsCache;
};

#endif
