	struct IDrawable
	{
		//////////////////
		// member types //
		//////////////////
		// member struct for typecasting objects
		struct DummyObject
		{
			void Draw() const { }
		};
		// itable type for this interface
		template<typename SELF> struct ITable
		{
			void(SELF::*Draw)() const;
		};
		// member struct for typecasting itable
		struct DummyITable : public ITable<DummyObject>
		{
			// empty
		};
		///////////////////
		// member fields //
		///////////////////
		DummyObject* mpObject;
		DummyITable* mpTable;
		//////////////////////
		// member functions //
		//////////////////////
		// default ctor
		IDrawable() { Assign(NULL, NULL); }
		// arbitrary initializing ctor
		template<typename _T> IDrawable(_T& x) { Assign(&x, GetStaticITable<_T>()); }
		// copy ctor specialization
		template<> IDrawable(const IDrawable& x) { Assign(x.mpObject, x.mpTable); }
		// arbitrary assignment operator
		template<typename _T> IDrawable& operator=(_T& x) { Assign(&x, GetStaticITable<_T>()); return *this; }
		// copy assignment specialization
		template<> IDrawable& operator=(const IDrawable& x) { Assign(x.mpObject, x.mpTable); return *this; }
		// assignment implementation
		void Assign(void* pObject, void* pTable) { mpObject = (DummyObject*)pObject; mpTable = (DummyITable*)pTable; }
		// static itable creation function
		template<typename _T> ITable<_T>* GetStaticITable()
		{
			static ITable<_T> itable =
			{
				&_T::Draw
			};
			return &itable;
		}
		/////////////////////////
		// interface functions //
		/////////////////////////
		void Draw() const { (mpObject->*(mpTable->Draw))(); }
	};
	struct IPosition
	{
		//////////////////
		// member types //
		//////////////////
		// member struct for typecasting objects
		struct DummyObject
		{
			Point GetPos() const { }
		};
		// itable type for this interface
		template<typename SELF> struct ITable
		{
			Point(SELF::*GetPos)() const;
		};
		// member struct for typecasting itable
		struct DummyITable : public ITable<DummyObject>
		{
			// empty
		};
		///////////////////
		// member fields //
		///////////////////
		DummyObject* mpObject;
		DummyITable* mpTable;
		//////////////////////
		// member functions //
		//////////////////////
		// default ctor
		IPosition() { Assign(NULL, NULL); }
		// arbitrary initializing ctor
		template<typename _T> IPosition(_T& x) { Assign(&x, GetStaticITable<_T>()); }
		// copy ctor specialization
		template<> IPosition(const IPosition& x) { Assign(x.mpObject, x.mpTable); }
		// arbitrary assignment operator
		template<typename _T> IPosition& operator=(_T& x) { Assign(&x, GetStaticITable<_T>()); return *this; }
		// copy assignment specialization
		template<> IPosition& operator=(const IPosition& x) { Assign(x.mpObject, x.mpTable); return *this; }
		// assignment implementation
		void Assign(void* pObject, void* pTable) { mpObject = (DummyObject*)pObject; mpTable = (DummyITable*)pTable; }
		// static itable creation function
		template<typename _T> ITable<_T>* GetStaticITable()
		{
			static ITable<_T> itable =
			{
				&_T::GetPos
			};
			return &itable;
		}
		/////////////////////////
		// interface functions //
		/////////////////////////
		Point GetPos() const { return (mpObject->*(mpTable->GetPos))(); }
	};
	struct IMoveable
	{
		//////////////////
		// member types //
		//////////////////
		// member struct for typecasting objects
		struct DummyObject
		{
			void MoveTo(const Point& x) { }
			void MoveBy(const Point& x) { }
		};
		// itable type for this interface
		template<typename SELF> struct ITable
		{
			void(SELF::*MoveTo)(const Point&);
			void(SELF::*MoveBy)(const Point&);
		};
		// member struct for typecasting itable
		struct DummyITable : public ITable<DummyObject>
		{
			// empty
		};
		///////////////////
		// member fields //
		///////////////////
		DummyObject* mpObject;
		DummyITable* mpTable;
		//////////////////////
		// member functions //
		//////////////////////
		// default ctor
		IMoveable() { Assign(NULL, NULL); }
		// arbitrary initializing ctor
		template<typename _T> IMoveable(_T& x) { Assign(&x, GetStaticITable<_T>()); }
		// copy ctor specialization
		template<> IMoveable(const IMoveable& x) { Assign(x.mpObject, x.mpTable); }
		// arbitrary assignment operator
		template<typename _T> IMoveable& operator=(_T& x) { Assign(&x, GetStaticITable<_T>()); return *this; }
		// copy assignment specialization
		template<> IMoveable& operator=(const IMoveable& x) { Assign(x.mpObject, x.mpTable); return *this; }
		// assignment implementation
		void Assign(void* pObject, void* pTable) { mpObject = (DummyObject*)pObject; mpTable = (DummyITable*)pTable; }
		// static itable creation function
		template<typename _T> ITable<_T>* GetStaticITable()
		{
			static ITable<_T> itable =
			{
				&_T::MoveTo
				, &_T::MoveBy
			};
			return &itable;
		}
		/////////////////////////
		// interface functions //
		/////////////////////////
		void MoveTo(const Point& x) { (mpObject->*(mpTable->MoveTo))(x); }
		void MoveBy(const Point& x) { (mpObject->*(mpTable->MoveBy))(x); }
	};
	struct ISizeable
	{
		//////////////////
		// member types //
		//////////////////
		// member struct for typecasting objects
		struct DummyObject
		{
			void SetSize(int x) { }
			int GetSize() const { }
		};
		// itable type for this interface
		template<typename SELF> struct ITable
		{
			void(SELF::*SetSize)(int);
			int(SELF::*GetSize)() const;
		};
		// member struct for typecasting itable
		struct DummyITable : public ITable<DummyObject>
		{
			// empty
		};
		///////////////////
		// member fields //
		///////////////////
		DummyObject* mpObject;
		DummyITable* mpTable;
		//////////////////////
		// member functions //
		//////////////////////
		// default ctor
		ISizeable() { Assign(NULL, NULL); }
		// arbitrary initializing ctor
		template<typename _T> ISizeable(_T& x) { Assign(&x, GetStaticITable<_T>()); }
		// copy ctor specialization
		template<> ISizeable(const ISizeable& x) { Assign(x.mpObject, x.mpTable); }
		// arbitrary assignment operator
		template<typename _T> ISizeable& operator=(_T& x) { Assign(&x, GetStaticITable<_T>()); return *this; }
		// copy assignment specialization
		template<> ISizeable& operator=(const ISizeable& x) { Assign(x.mpObject, x.mpTable); return *this; }
		// assignment implementation
		void Assign(void* pObject, void* pTable) { mpObject = (DummyObject*)pObject; mpTable = (DummyITable*)pTable; }
		// static itable creation function
		template<typename _T> ITable<_T>* GetStaticITable()
		{
			static ITable<_T> itable =
			{
				&_T::SetSize
				, &_T::GetSize
			};
			return &itable;
		}
		/////////////////////////
		// interface functions //
		/////////////////////////
		void SetSize(int x) { (mpObject->*(mpTable->SetSize))(x); }
		int GetSize() const { return (mpObject->*(mpTable->GetSize))(); }
	};
	struct IShape
	{
		//////////////////
		// member types //
		//////////////////
		// member struct for typecasting objects
		struct DummyObject
		{
			void Draw() const { }
			void MoveTo(const Point& x) { }
			void MoveBy(const Point& x) { }
			void SetSize(int x) { }
			int GetSize() const { }
		};
		// itable type for this interface
		template<typename SELF> struct ITable
		{
			void(SELF::*Draw)() const;
			void(SELF::*MoveTo)(const Point&);
			void(SELF::*MoveBy)(const Point&);
			void(SELF::*SetSize)(int);
			int(SELF::*GetSize)() const;
		};
		// member struct for typecasting itable
		struct DummyITable : public ITable<DummyObject>
		{
			// empty
		};
		///////////////////
		// member fields //
		///////////////////
		DummyObject* mpObject;
		DummyITable* mpTable;
		//////////////////////
		// member functions //
		//////////////////////
		// default ctor
		IShape() { Assign(NULL, NULL); }
		// arbitrary initializing ctor
		template<typename _T> IShape(_T& x) { Assign(&x, GetStaticITable<_T>()); }
		// copy ctor specialization
		template<> IShape(const IShape& x) { Assign(x.mpObject, x.mpTable); }
		// arbitrary assignment operator
		template<typename _T> IShape& operator=(_T& x) { Assign(&x, GetStaticITable<_T>()); return *this; }
		// copy assignment specialization
		template<> IShape& operator=(const IShape& x) { Assign(x.mpObject, x.mpTable); return *this; }
		// assignment implementation
		void Assign(void* pObject, void* pTable) { mpObject = (DummyObject*)pObject; mpTable = (DummyITable*)pTable; }
		// static itable creation function
		template<typename _T> ITable<_T>* GetStaticITable()
		{
			static ITable<_T> itable =
			{
				&_T::Draw
				, &_T::MoveTo
				, &_T::MoveBy
				, &_T::SetSize
				, &_T::GetSize
			};
			return &itable;
		}
		/////////////////////////
		// interface functions //
		/////////////////////////
		void Draw() const { (mpObject->*(mpTable->Draw))(); }
		void MoveTo(const Point& x) { (mpObject->*(mpTable->MoveTo))(x); }
		void MoveBy(const Point& x) { (mpObject->*(mpTable->MoveBy))(x); }
		void SetSize(int x) { (mpObject->*(mpTable->SetSize))(x); }
		int GetSize() const { return (mpObject->*(mpTable->GetSize))(); }
	};
