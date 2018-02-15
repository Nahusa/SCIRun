/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include <Interface/Modules/Math/BasicPlotterDialog.h>
#include <Modules/Math/BasicPlotter.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
#include <QtGui>

#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_symbol.h>
#include <qwt_math.h>

using namespace SCIRun::Gui;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Modules::Math;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Algorithms::Math;
using namespace SCIRun::Core::Datatypes;

BasicPlotterDialog::BasicPlotterDialog(const std::string& name, ModuleStateHandle state,
	QWidget* parent/* = 0*/)
	: ModuleDialogGeneric(state, parent)
{
	setupUi(this);
	setWindowTitle(QString::fromStdString(name));
	fixSize();

	addDoubleSpinBoxManager(verticalAxisSpinBox_, Parameters::VerticalAxisPosition);
  addDoubleSpinBoxManager(horizontalAxisSpinBox_, Parameters::HorizontalAxisPosition);
	//addCheckBoxManager(verticalAxisGroupBox_, Parameters::VerticalAxisVisible);
  //addCheckBoxManager(horizontalAxisGroupBox_, Parameters::HorizontalAxisVisible);
	addLineEditManager(titleLineEdit_, Parameters::PlotTitle);
	addLineEditManager(dataLineEdit_, Parameters::DataTitle);
	addLineEditManager(xAxisLineEdit_, Parameters::XAxisLabel);
	addLineEditManager(yAxisLineEdit_, Parameters::YAxisLabel);

	connect(showPlotPushButton_, SIGNAL(clicked()), this, SLOT(showPlot()));
	connect(dataColorPushButton_, SIGNAL(clicked()), this, SLOT(assignDataColor()));

	dataColors_.push_back(Qt::red);
}

BasicPlotterDialog::~BasicPlotterDialog()
{
	delete plotDialog_;
}

void BasicPlotterDialog::pullSpecial()
{
  auto data = transient_value_cast<DenseMatrixHandle>(state_->getTransientValue(Variables::InputMatrix));

	if (plotDialog_ && plotDialog_->isVisible())
		updatePlot();
}

void BasicPlotterDialog::showPlot()
{
	if (!plotDialog_)
	{
		plotDialog_ = new QDialog;
		plotDialog_->setStyleSheet(styleSheet());
		auto layout = new QHBoxLayout( plotDialog_ );
		layout->setContentsMargins( 5, 5, 5, 5 );
		plot_ = new Plot(this);
		layout->addWidget( plot_ );
		plotDialog_->resize( 600, 400 );
	}

	updatePlot();
	plotDialog_->show();
	plotDialog_->raise();
}

void BasicPlotterDialog::updatePlot()
{
	plot_->setTitle(titleLineEdit_->text());
	plot_->setAxisTitle(QwtPlot::xBottom, xAxisLineEdit_->text());
	plot_->setAxisTitle(QwtPlot::yLeft, yAxisLineEdit_->text());
	plot_->makeHorizontalAxis(horizontalAxisGroupBox_->isChecked(), horizontalAxisSpinBox_->value());
	plot_->makeVerticalAxis(verticalAxisGroupBox_->isChecked(), verticalAxisSpinBox_->value());
	auto data = transient_value_cast<DenseMatrixHandle>(state_->getTransientValue(Variables::InputMatrix));
	if (data)
		plot_->makeCurve(data, dataLineEdit_->text(), dataColors_[0]);
	plot_->replot();
}

Plot::Plot(QWidget *parent) : QwtPlot( parent )
{
  setAutoFillBackground( true );

  insertLegend( new QwtLegend(), QwtPlot::RightLegend );

  auto canvas = new QwtPlotCanvas(this);
  canvas->setLineWidth( 1 );
  canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
  canvas->setBorderRadius( 15 );

  setCanvas( canvas );

  // panning with the left mouse button
  ( void ) new QwtPlotPanner( canvas );

  // zoom in/out with the wheel
  ( void ) new QwtPlotMagnifier( canvas );
}

void Plot::makeVerticalAxis(bool show, double position)
{
	if (show)
	{
		delete verticalAxis_;
		verticalAxis_ = new QwtPlotMarker();
		verticalAxis_->setLabel("x = " + QString::number(position));
		verticalAxis_->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
		verticalAxis_->setLabelOrientation( Qt::Vertical );
		verticalAxis_->setLineStyle( QwtPlotMarker::VLine );
		verticalAxis_->setLinePen( Qt::black, 0, Qt::DashDotLine );
		verticalAxis_->setXValue( position );
		verticalAxis_->attach( this );
	}
	else
	{
		if (verticalAxis_)
			verticalAxis_->detach();
	}
}

void Plot::makeHorizontalAxis(bool show, double position)
{
	if (show)
	{
		delete horizontalAxis_;
		horizontalAxis_ = new QwtPlotMarker();
		horizontalAxis_->setLabel("y = " + QString::number(position));
		horizontalAxis_->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
		horizontalAxis_->setLineStyle( QwtPlotMarker::HLine );
		horizontalAxis_->setYValue( position );
		horizontalAxis_->attach( this );
	}
	else
	{
		if (horizontalAxis_)
			horizontalAxis_->detach();
	}
}

void Plot::makeCurve(DenseMatrixHandle data, const QString& title, const QColor& color)
{
	if (curve_)
	{
		curve_->detach();
		delete curve_;
	}

  auto x = data->col(0);
  auto y = data->col(1);
  double maxX = x.maxCoeff();
  double maxY = y.maxCoeff();
  double minX = x.minCoeff();
  double minY = y.minCoeff();
	setAxisScale( xBottom, minX, maxX );
  setAxisScale( yLeft, minY, maxY );

	QPolygonF points;
  for (int i = 0; i < data->nrows(); ++i)
  {
    points << QPointF(x(i), y(i));
  }

  curve_ = new QwtPlotCurve();
  curve_->setPen(color, 2),
  curve_->setTitle(title);
  curve_->setRenderHint( QwtPlotItem::RenderAntialiased, true );
	curve_->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
  curve_->attach(this);
  curve_->setSamples( points );
}

void BasicPlotterDialog::assignDataColor()
{
  auto newColor = QColorDialog::getColor(dataColors_[0], this, "Choose data color");
  if (newColor.isValid())
  {
		dataColors_[0] = newColor;
		updatePlot();
  }
}
