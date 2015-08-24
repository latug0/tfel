/*!
 * \file   MakefileGenerator.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 août 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<set>
#include<ostream>
#include<sstream>
#include<fstream>
#include<iterator>
#include<algorithm>

#include"TFEL/System/System.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/SearchFile.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/GeneratorOptions.hxx"
#include"MFront/MakefileGenerator.hxx"

namespace mfront{

  static std::string
  sortLibraryList(const std::string& lib)
  {
    std::istringstream tokenizer{lib};
    auto libs = std::vector<std::string>{};
    auto vres = std::vector<std::string>{};
    copy(std::istream_iterator<std::string>(tokenizer),
	 std::istream_iterator<std::string>(),back_inserter(libs));
    auto p  = libs.crbegin();
    const auto pe = libs.crend();
    auto res = std::string{};
    while(p!=pe){
      if(find(vres.begin(),vres.end(),*p)==vres.end()){
	vres.push_back(*p);
	res = *p+" "+res; 
      }
      ++p;
    }
    return res;
  } // end of sortLibraryList
  
  static std::string
  getLibraryLinkDependencies(std::ostream& m,
			     const TargetsDescription& t,
			     const GeneratorOptions& o,
			     const std::string& name)
  {
    if(!describes(t,name)){
      throw(std::runtime_error("getLibraryDependencies : no library named '"+name+"'.\n"
			       "Internal Error."));
    }
    const auto& l = t[name];
    if(l.ldflags.empty()){
      return {};
    }
    auto res = std::string{};
    m << "-L. ";
    for(const auto& d : l.ldflags){
      if(!d.empty()){
	if(d.size()>2){
	  if(d.substr(0,2)=="-l"){
	    if(describes(t,"lib"+d.substr(2))){
	      if(!o.melt){
		res += getLibraryLinkDependencies(m,t,o,d) + " " + d + " ";
	      }
	    } else {
	      res += d + " ";
	    }
	  } else {
	    res += d + " ";
	  }
	} else {
	  res += d + " ";
	}
      }
    }
    return sortLibraryList(res);
  } // end of getLibraryLinkDependencies(const std::string&)
  
  static std::pair<bool,std::pair<std::string,std::string> >
  getLibraryDependencies(const TargetsDescription& t,
			 const GeneratorOptions& o,
			 const std::string& name)
  {
    using namespace std;
    const auto& l = t[name];
    auto res = pair<bool,pair<string,string> >{};
    res.first = false;
    for(const auto& s : l.sources){
      if(s.size()>4){
	const auto ext = s.substr(s.size()-4);
	if((ext==".cpp")||(ext==".cxx")){
	  res.first = true;
	  res.second.first += s.substr(0,s.size()-4)+".o ";
	}
      }
      if(s.size()>2){
	if(s.substr(s.size()-2)==".c"){
	  res.second.first += s.substr(0,s.size()-2)+".o ";
	}
      }
    }
    for(const auto& d : l.ldflags){
      if(d.size()>2){
	if(d.substr(0,2)=="-l"){
	  auto lib = d.substr(2);
	  if(describes(t,lib)){
	    if(o.melt){
	      auto dep = getLibraryDependencies(t,o,"lib"+lib);
	      res.first = res.first || dep.first;
	      res.second.first  += dep.second.first;
	      res.second.second += dep.second.second;
	    } else {
	      res.second.second += "lib";
	      res.second.second += lib;
	      res.second.second +=  + "."+l.suffix+" ";
	    }
	  }
	}
      }
    }
    return res;
  } // end of getLibraryDependencies
  
  void
  generateMakeFile(const TargetsDescription& t,
		   const GeneratorOptions& o,
		   const std::string& d,
		   const std::string& f){
    using namespace std;
    using namespace tfel::system;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      auto& log = getLogStream();
      log << "generating Makefile\n";
    }
    auto& mlock = MFrontLock::getMFrontLock();
    mlock.lock();
    try{
      const auto env_cc   = ::getenv("CC");
      const auto env_cxx  = ::getenv("CXX");
      const auto inc      = ::getenv("INCLUDES");
      const auto cxxflags = ::getenv("CXXFLAGS");
      const auto cflags   = ::getenv("CFLAGS");
      const auto ldflags  = ::getenv("LDFLAGS");
      const auto sb  = o.silentBuild ? "@" : "";
      const auto cxx = (env_cxx==nullptr) ? "$(CXX)" : env_cxx;
      const auto cc  = (env_cc ==nullptr) ? "$(CC)"  : env_cc;
      auto mfile = d+dirStringSeparator()+f;
      ofstream m(mfile);
      m.exceptions(ios::badbit|ios::failbit);
      if(!m){
	throw(runtime_error("generateMakeFile : can't open file '"+mfile+"'"));
      }
      auto cppSources = set<string>{};
      auto cSources   = set<string>{};
      for(const auto& l : t){
	for(const auto& src : l.sources){
	  if(src.size()>4){
	    if((src.substr(src.size()-4)==".cpp")||
	       (src.substr(src.size()-4)==".cxx")){
	      cppSources.insert(src);
	    }
	  }
	  if(src.size()>2){
	    if(src.substr(src.size()-2)==".c"){
	      cSources.insert(src);
	    }
	  }
	}
      }
      m << "# Makefile generated by mfront.\n"
	<< MFrontHeader::getHeader("# ") << "\n";
      m << "export LD_LIBRARY_PATH:=$(PWD):$(LD_LIBRARY_PATH)\n\n";
      // COMPILERS
      if(env_cc!=nullptr){
	m << "CC := "  << env_cc  << "\n";
      }
      if(env_cxx!=nullptr){
	m << "CXX := " << env_cxx << "\n";
      }
      if((env_cc!=nullptr)||(env_cxx!=nullptr)){
	m << endl;
      }
      // INCLUDES
      m << "INCLUDES := ";
      if(inc!=nullptr){
	m << inc << " ";
      }
      m << "-I../include `tfel-config --includes`";
      // cpp flags
      vector<string> tmp_cppflags;
      for(const auto& l:t){
	tmp_cppflags.insert(tmp_cppflags.end(),
			    l.cppflags.begin(),l.cppflags.end());
      }
      if(!tmp_cppflags.empty()){
	m << " \\\n";
	for(auto p7=tmp_cppflags.begin();p7!=tmp_cppflags.end();){
	  m << "\t     " << *p7;
	  if(++p7!=tmp_cppflags.end()){
	    m << " \\\n";
	  }
	}
      }
      // adding the mfront search path to the include files
      if(!SearchFile::getSearchPaths().empty()){
	const auto& paths = SearchFile::getSearchPaths();
	for(const auto& path : paths){
	  m << "\\\n\t     -I" << path;
	}
      }
      //
      m << endl << endl;
      // LDFLAGS
      if(ldflags!=nullptr){
	m << "LDFLAGS := " << ldflags  << endl;
      }
      // CXXFLAGS
      if(!cppSources.empty()){
	m << "CXXFLAGS := -Wall -Wfatal-errors ";
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
	m << "-ansi ";
#endif /* __CYGWIN__ */
	if(cxxflags!=nullptr){
	  m << cxxflags << " ";
	} else if(o.oflags0||o.oflags||o.oflags2){
	  if(o.oflags2){
	    m << "`tfel-config --compiler-flags --oflags --oflags2` ";
	  } else if(o.oflags){
	    m << "`tfel-config --compiler-flags --oflags` ";
	  } else {
	    m << "`tfel-config --compiler-flags --oflags0` ";
	  }
	} else {
	  m << "-O2 `tfel-config --compiler-flags`";
	}
	if(o.sys=="win32"){
	  m << "-DWIN32 -DMFRONT_COMPILING $(INCLUDES) \n\n";
	} else {
	  m << "-fPIC $(INCLUDES) \n\n";
	}
      }
      // CFLAGS
      if(!cSources.empty()){
	m << "CFLAGS := -W -Wall -Wfatal-errors ";
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
	m << "-ansi -std=c99 ";
#endif /* __CYGWIN__ */
	if(cflags!=nullptr){
	  m << cflags << " ";
	} else if(o.oflags0||o.oflags||o.oflags2){
	  if(o.oflags2){
	    m << "`tfel-config --oflags --oflags2` ";
	  } else if(o.oflags){
	    m << "`tfel-config --compiler-flags --oflags` ";
	  } else {
	    m << "`tfel-config --compiler-flags --oflags0` ";
	  }
	} else {
	  m << "-O2 ";
	}
	if(o.sys=="win32"){
	  m << "-DWIN32 -DMFRONT_COMPILING $(INCLUDES)\n\n";
	} else {
	  m << "-fPIC $(INCLUDES)\n\n";
	}
      }
      // sources list
      if(!cppSources.empty()){
	m << "SRCCXX = ";
	auto p4=cppSources.begin();
	while(p4!=cppSources.end()){
	  m << *p4;
	  if(++p4!=cppSources.end()){
	    m << " ";
	  }
	}
	m << "\n\n";
      }
      if(!cSources.empty()){
	m << "SRC = ";
	auto p4=cSources.begin();
	while(p4!=cSources.end()){
	  m << *p4;
	  if(++p4!=cSources.end()){
	    m << " ";
	  }
	}
	m << "\n\n";
      }
      if(!o.nodeps){
	if(!cppSources.empty()){
	  m << "makefiles1 = $(SRCCXX:.cxx=.d)\n";
	  m << "makefiles2 = $(makefiles1:.cpp=.d)\n";
	}
	if(!cSources.empty()){
	  m << "makefiles3 = $(SRC:.c=.d)\n";
	}
	m << "makefiles  =";
	if(!cppSources.empty()){
	  m << " $(makefiles2)";
	}
	if(!cSources.empty()){
	  m << " $(makefiles3)";
	}
      }
      m << "\n\n";
      m << ".PHONY = ";
      m << "all clean ";
      for(const auto& l : t){
	m << l.name << "." << l.suffix << " ";
      }
      for(const auto& target : t.specific_targets){
	if((target.first!="all")&&(target.first!="clean")){
	  m << target.first << " ";
	}
      }
      m << "\n\n";
      m << "all : ";
      for(const auto& l : t){
	m << l.name << "." << l.suffix << " ";
      }
      auto p5=t.specific_targets.find("all");
      if(p5!=t.specific_targets.end()){
	copy(p5->second.first.begin(),p5->second.first.end(),
	     ostream_iterator<string>(m," "));
	m << "\n";
	for(const auto& cmd : p5->second.second){
	  m << "\t" << cmd << "\n";
	}
      }
      m << "\n";
      for(const auto& target : t.specific_targets){
	if((target.first!="all")&&(target.first!="clean")){
	  m << target.first << " : ";
	  copy(target.second.first.begin(),target.second.first.end(),
	       ostream_iterator<string>(m," "));
	  m << endl;
	  for(const auto& cmd : target.second.second){
	    m << "\t" << cmd << endl;
	  }
	  m << "\n";
	}
      }
      for(const auto& l : t){
	m << l.name << "." << l.suffix << " : ";
	auto dep = getLibraryDependencies(t,o,l.name);
	const auto hasCxxSources = dep.first;
	if(!dep.second.first.empty()){
	  m << dep.second.first;
	} 
	if(!dep.second.second.empty()){
	  m << sortLibraryList(dep.second.second);
	}
	m << "\n\t";
	if(hasCxxSources){
	  m << sb << cxx << " ";
	} else {
	  m << sb << cc  << " ";
	}
	if(ldflags!=nullptr){
	  m << "$(LDFLAGS) ";
	}
	if(o.sys=="win32"){
	  m << "-shared -Wl,--add-stdcall-alias,--out-implib," << l.name << "_dll.a,-no-undefined ";
	} else if(o.sys=="apple"){
	  m << "-bundle ";
	} else {
	  m << "-shared ";
	}
	m << "$^  -o $@ ";
	m << getLibraryLinkDependencies(m,t,o,l.name);
	m << "\n\n";
      }
      m << "clean : ";
      p5=t.specific_targets.find("clean");
      if(p5!=t.specific_targets.end()){
	copy(p5->second.first.begin(),
	     p5->second.first.end(),ostream_iterator<string>(m," "));
      }
      m << "\n";
      if(o.sys=="win32"){
	m << "\t" << sb << "rm -f *.o *.dll *.d\n";
      } else if(o.sys=="apple"){
	m << "\t" << sb << "rm -f *.o *.bundle *.d\n";
      } else {
	m << "\t" << sb << "rm -f *.o *.so *.d\n";
      }
      if(p5!=t.specific_targets.end()){
	for(const auto& cmd : p5->second.second){
	  m << "\t" << cmd << endl;
	}
      }
      m << "\n";
      if(!o.nodeps){
	m << "-include $(makefiles)\n\n";
      }
      // generic rules for objects file generation
      if(!cppSources.empty()){
	m << "%.o:%.cxx\n";
	m << "\t" << sb << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
	m << "%.o:%.cpp\n";
	m << "\t" << sb << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
      }
      if(!cSources.empty()){
	m << "%.o:%.c\n";
	m << "\t" << sb << cc << " $(CFLAGS) $< -o $@ -c\n\n";
      }
      if(!o.nodeps){
	if(!cppSources.empty()){
	  m << "%.d:%.cxx\n";
	  m << "\t@set -e; rm -f $@;					  \\\n";
	  m << "\t$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \\\n";
	  m << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; \\\n";
	  m << "\trm -f $@.$$$$\n\n";
	  m << "%.d:%.cpp\n";
	  m << "\t@set -e; rm -f $@;					  \\\n";
	  m << "\t$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \\\n";
	  m << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; \\\n";
	  m << "\trm -f $@.$$$$\n\n";
	}
	if(!cSources.empty()){
	  m << "%.d:%.c\n";
	  m << "\t@set -e; rm -f $@;					  \\\n";
	  m << "\t$(CC) -M $(CFLAGS) $< > $@.$$$$; \\\n";
	  m << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; \\\n";
	  m << "\trm -f $@.$$$$\n";
	}
      }
      m.close();
    }
    catch(...){
      mlock.unlock();
      throw;
    }
    mlock.unlock();
  }
  
} // end of namespace mfront