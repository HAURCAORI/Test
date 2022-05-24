#include "rtplot.h"
#include <iomanip>
#include <sstream>

int getDigit(FLOAT value) {
    return (int) ceil(log10(fabs(value)))+1;
}

double roundValue(double value, int digits){
    if (value == 0.0) // otherwise it will return 'nan' due to the log10() of zero
        return 0.0;

    double factor = pow(10.0, digits - ceil(log10(fabs(value))));
    return round(value * factor) / factor;
}

std::string roundString(double value, int digits)
{
    std::stringstream stream;
    if (value == 0.0) {
        stream << std::fixed << std::setprecision(digits-1)<< 0.0;
        return stream.str();
    }

    double n = log10(fabs(value));
    double factor = pow(10.0, digits - ceil(n));
    if(n < 0) {
        stream << std::fixed << std::setprecision(-((int) n)+digits) << (round(value * factor) / factor);
        return stream.str();
    }
    stream << std::fixed << std::setprecision((digits-((int) n)-1) > 0 ? digits-((int) n)-1 : 0) << (round(value * factor) / factor);
    return stream.str();
}

namespace rtplot {
bool DataSet::deleteData(std::string name) {
    for(auto it = datas.begin(); it != datas.end();) {
        if(it->getName() == name) { it = datas.erase(it); return true; }
        it++;
    }
    return false;
}
bool DataSet::deleteData(unsigned int index) {
    if(datas.size() > index ) {
        datas.erase(std::next(datas.begin(), index));
        return true;
    }
    return false;
}

Data* DataSet::getData(std::string name) {
    for(auto it = datas.begin(); it != datas.end(); ++it) {
        if(it->getName() == name) { return &(*it); }
    }
    return nullptr;
}
Data* DataSet::getData(unsigned int index) {
    if(datas.size() > index ) {
        return &datas[index];
    }
    return nullptr;
}

void printDataByType(Data& data) {
    if(data.getType() == DataType::SINGLE_INT) {
        auto d = static_cast<const std::vector<INT>*>(data.getData());
        printVector(*d);
    } else if(data.getType() == DataType::SINGLE_FLOAT) {
        auto d = static_cast<const std::vector<FLOAT>*>(data.getData());
        printVector(*d);
    } else if(data.getType() == DataType::SINGLE_STRING) {
        auto d = static_cast<const std::vector<STRING>*>(data.getData());
        printVector(*d);
    } else if(data.getType() == DataType::PAIR_FLOAT_FLOAT) {
        auto d = static_cast<const std::vector<Pair<FLOAT,FLOAT>>*>(data.getData());
        printVector<FLOAT,FLOAT>(*d);
    } else if(data.getType() == DataType::PAIR_STRING_INT) {
        auto d = static_cast<const std::vector<Pair<STRING,INT>>*>(data.getData());
        printVector<STRING,INT>(*d);
    } else if(data.getType() == DataType::PAIR_STRING_FLOAT) {
        auto d = static_cast<const std::vector<Pair<STRING,FLOAT>>*>(data.getData());
        printVector<STRING,FLOAT>(*d);
    }
}

void DataSet::printData(std::string name) {
    for(auto it = datas.begin(); it != datas.end();++it) {
        if(it->getName() == name) {
            std::cout << "DataSet ['" << name << "'] :" << std::endl;
            printDataByType(*it);
        }
    }
}
void DataSet::printData(unsigned int index) {
    if(datas.size() <= index) { return; }
    std::cout << "DataSet ['" << datas[index].getName() << "'] :" << std::endl;
    printDataByType(datas[index]);
}
void DataSet::printAll() {
    for(auto it = datas.begin(); it != datas.end();++it) {
        std::cout << "DataSet ['" << (*it).getName() << "'] :" << std::endl;
        printDataByType(*it);
    }
}



void rtplot::init() {
    Simd::FillValue(m_view,255);
    Simd::FillValue(m_plot_view,255);
    Simd::FillValue(m_title_view,255);
    Simd::FillValueBgra(view_x_axis,255,255,0,255);
    Simd::FillValueBgra(view_y_axis,255,255,0,255);
    drawPlot();
}


void rtplot::drawPlot() {
    if(title_changed){
        Point tfm =m_title_font.Measure(m_title);
        m_title_font.Draw(m_title_view,m_title, Point((m_title_size.width-tfm.x)/2, (m_title_size.height-tfm.y)/2),color_text);
        title_changed = false;
    }

    //좌표 축
    double height_per_value = m_plot_view.height/(y_axis.max_value - y_axis.min_value);
    double width_per_value = m_plot_view.width/(x_axis.max_value - x_axis.min_value);
    {
        Simd::FillValue(m_plot_view,255);
        Simd::FillValueBgra(view_x_axis,255,255,0,255);
        Simd::FillValueBgra(view_y_axis,255,255,0,255);
        if(y_axis.show) {
            double delta_y_major = (double) m_plot_view.height/y_axis.major_tick;
            double delta_y_minor = (double) m_plot_view.height/y_axis.minor_tick;
            double approx_min_y = roundValue(y_axis.min_value,getDigit(y_axis.major_interval));
            for(size_t i = 0; i <= y_axis.minor_tick; i++) {
                size_t ty = delta_y_minor*i + height_per_value*(approx_min_y-y_axis.min_value);
                Simd::DrawingLine(view_y_axis,origin_y_axis(0, ty),origin_y_axis(3, ty),color_axis_minor);
            }
            for(size_t i = 0; i <= y_axis.major_tick; i++) {
                std::string y_value = roundString(approx_min_y+ y_axis.major_interval*i,y_axis.digit);
                size_t ty = delta_y_major*i + height_per_value*(approx_min_y-y_axis.min_value);
                Simd::DrawingLine(view_y_axis,origin_y_axis(0, ty),origin_y_axis(10, ty),color_axis);
                if(show_grid) Simd::DrawingLine(m_plot_view,origin_plot(1,ty),origin_plot(m_plot_size.width-2,ty),color_grid);
                Point tp =font.Measure(y_value);
                font.Draw(view_y_axis,y_value,origin_y_axis(tp.x + 10,ty + tp.y/2),color_text);
            }
        }

        if(x_axis.show) {
            double delta_x_major = (double) m_plot_view.width/x_axis.major_tick;
            double delta_x_minor = (double) m_plot_view.width/x_axis.minor_tick;
            double approx_min_x = roundValue(x_axis.min_value,getDigit(x_axis.major_interval));
            for(size_t i = 0; i <= x_axis.minor_tick; i++) {
                size_t tx = delta_x_minor*i + width_per_value*(approx_min_x-x_axis.min_value);
                Simd::DrawingLine(view_x_axis,origin_x_axis(tx, 0),origin_x_axis(tx, 3),color_axis_minor);
            }
            for(size_t i = 0; i <= x_axis.major_tick; i++) {
                std::string x_value = roundString(approx_min_x+ x_axis.major_interval*i,x_axis.digit);
                size_t tx = delta_x_major*i + width_per_value*(approx_min_x-x_axis.min_value);
                Simd::DrawingLine(view_x_axis,origin_x_axis(tx, 0),origin_x_axis(tx, 10),color_axis);
                if(show_grid) Simd::DrawingLine(m_plot_view,origin_plot(tx,1),origin_plot(tx,m_plot_size.height-2),color_grid);
                Point tp =font.Measure(x_value);
                font.Draw(view_x_axis,x_value,origin_x_axis(tx - tp.x/2,tp.y + 10),color_text);
            }
        }
    }
    //중심 선
    if(y_axis.min_value < 0 && y_axis.max_value > 0) {
        size_t ty = (0.0-y_axis.min_value)*height_per_value;
        Simd::DrawingLine(m_plot_view,origin_plot(1,ty),origin_plot(m_plot_size.width-2,ty),color_axis);
    }
    if(x_axis.min_value < 0 && x_axis.max_value > 0) {
        size_t tx = (0.0 - x_axis.min_value)*width_per_value;
        Simd::DrawingLine(m_plot_view,origin_plot(tx,1),origin_plot(tx,m_plot_size.height-2),color_axis);
    }


    //plot 외부선
    Simd::DrawingLine(m_plot_view,0,0,0,m_plot_size.height,color_axis);
    Simd::DrawingLine(m_plot_view,0,m_plot_size.height-1,m_plot_size.width-1,m_plot_size.height-1,color_axis);
    Simd::DrawingLine(m_plot_view,0,0,m_plot_size.width,0,color_axis);
    Simd::DrawingLine(m_plot_view,m_plot_size.width-1,0,m_plot_size.width-1,m_plot_size.height-1,color_axis);

}

void rtplot::updatePlot() {
    Simd::Overlay(m_view,m_title_location.x,m_title_location.y,m_title_view);
    Simd::Overlay(m_view,x_axis.location.x, x_axis.location.y,view_x_axis);
    Simd::Overlay(m_view,y_axis.location.x, y_axis.location.y,view_y_axis);
    Simd::Overlay(m_view,m_plot_location.x,m_plot_location.y,m_plot_view);
}

void rtplot::movePlot(int x, int y)
{
    double value_per_width = (x_axis.max_value - x_axis.min_value) / m_plot_view.width;
    double value_per_height = (y_axis.max_value - y_axis.min_value) / m_plot_view.height;
    std::cout << x<< "/" << y << std::endl;
    x_axis.max_value -=  x * value_per_width;
    x_axis.min_value -=  x * value_per_width;
    y_axis.max_value +=  y * value_per_height;
    y_axis.min_value += y * value_per_height;

    drawPlot();
    updatePlot();
}

void rtplot::moveOrigin() {
    double middle_x = (x_axis.max_value + x_axis.min_value)/2;
    double middle_y = (y_axis.max_value + y_axis.min_value)/2;
    x_axis.max_value -= middle_x;
    x_axis.min_value -= middle_x;
    y_axis.max_value -= middle_y;
    y_axis.min_value -= middle_y;
    drawPlot();
    updatePlot();
}
}
