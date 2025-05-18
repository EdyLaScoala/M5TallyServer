////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>
//#include <CurieNeurons.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	namespace CurieNeuronRegisters
	{
		const int NEURONSIZE = 128;

		volatile uint8_t* PVP_REGISTER_BASE = (volatile uint8_t*)0xB0600000;
		volatile int*	const NM_NCR =      (volatile int*)(PVP_REGISTER_BASE + 0x00);
		volatile int*	const NM_COMP =     (volatile int*)(PVP_REGISTER_BASE + 0x04);
		volatile int*	const NM_LCOMP =    (volatile int*)(PVP_REGISTER_BASE + 0x08);
		volatile int*	const NM_DIST =     (volatile int*)(PVP_REGISTER_BASE + 0x0C);
		volatile int*	const NM_IDX =     	(volatile int*)(PVP_REGISTER_BASE + 0x0C);
		volatile int*	const NM_CAT =     	(volatile int*)(PVP_REGISTER_BASE + 0x10);
		volatile int*	const NM_AIF =     	(volatile int*)(PVP_REGISTER_BASE + 0x14);
		volatile int*	const NM_MINIF =    (volatile int*)(PVP_REGISTER_BASE + 0x18);
		volatile int*	const NM_MAXIF =    (volatile int*)(PVP_REGISTER_BASE + 0x1C);
		volatile int*	const NM_TESTCOMP = (volatile int*)(PVP_REGISTER_BASE + 0x20);
		volatile int*	const NM_TESTCAT =  (volatile int*)(PVP_REGISTER_BASE + 0x24);
		volatile int*	const NM_NID =  	(volatile int*)(PVP_REGISTER_BASE + 0x28);
		volatile int*	const NM_GCR =  	(volatile int*)(PVP_REGISTER_BASE + 0x2C);
		volatile int*	const NM_RSTCHAIN = (volatile int*)(PVP_REGISTER_BASE + 0x30);
		volatile int*	const NM_NSR =  	(volatile int*)(PVP_REGISTER_BASE + 0x34);
		volatile int*	const NM_FORGET =  	(volatile int*)(PVP_REGISTER_BASE + 0x3C);
		volatile int*	const NM_NCOUNT =  	(volatile int*)(PVP_REGISTER_BASE + 0x3C);
	}
//---------------------------------------------------------------------------
	class Arduino101CurieNeurons
	{
	public:
		inline void SystemStart()
		{
			int TempNSR = *CurieNeuronRegisters::NM_NSR;
			*CurieNeuronRegisters::NM_FORGET = 0;
			*CurieNeuronRegisters::NM_NSR = 16;
			for( int i=0; i < CurieNeuronRegisters::NEURONSIZE; i++ ) 
				*CurieNeuronRegisters::NM_TESTCOMP = 0;

			*CurieNeuronRegisters::NM_NSR = TempNSR;
		}

	public:
		inline void ForgetInputPin_o_Receive( void *_Data )
		{
			*CurieNeuronRegisters::NM_FORGET = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_CONTEXT,
		typename T_IsKNN,
		typename T_UseInfinityDistance
	> class Arduino101CurieNeuronsClassifier :
		public T_IsKNN,
		public T_UseInfinityDistance
	{
	public:
		_V_PROP_( IsKNN )
		_V_PROP_( UseInfinityDistance )

	public:
		TValueArray<float>	FFeaturesValue;

	public:
		inline constexpr uint32_t GetContext()
		{
			return C_CONTEXT;
		}

	public:
		void FillFeatures()
		{
			int ACount = MitovMin<uint32_t>( FFeaturesValue.Size, 128 );
			for( int i = 0; i < ACount; ++i )
			{
				float AValue = MitovConstrain( FFeaturesValue.Data[ i ], 0.0f, 1.0f ) * 255 + 0.5;
				if( i == ACount - 1 )
					*CurieNeuronRegisters::NM_LCOMP = AValue;

				else
					*CurieNeuronRegisters::NM_COMP = AValue;
			}
		}

		void Forget()
		{
			int ANCount = *CurieNeuronRegisters::NM_NCOUNT;
			int ATempNSR = *CurieNeuronRegisters::NM_NSR;
			*CurieNeuronRegisters::NM_NSR = 0x0010;
			*CurieNeuronRegisters::NM_RSTCHAIN = 0;
			int ACountOtherNeurons = 0;
			for( int i=0; i < ANCount; i++ )
			{
				int Temp = *CurieNeuronRegisters::NM_NCR;
				if( Temp && 0x7F != C_CONTEXT )
					++ACountOtherNeurons;

				Temp = *CurieNeuronRegisters::NM_CAT; // Move to the next neuron
			}

			if( ! ACountOtherNeurons )
				*CurieNeuronRegisters::NM_FORGET = 0;

			else
			{
				int *ANeuronData = new int[ ACountOtherNeurons * ( 4 + ( 128 / 2 )) ];

				*CurieNeuronRegisters::NM_RSTCHAIN = 0;
				int *ACurrentPtr = ANeuronData;
				for( int i=0; i < ANCount; ++i )
				{
					int ATemp = *CurieNeuronRegisters::NM_NCR;
					if( ATemp && 0x7F == C_CONTEXT )
						ATemp = *CurieNeuronRegisters::NM_CAT; // Move to the next neuron

					else
					{
						*ACurrentPtr++ = ATemp; // Save context
						uint8_t *AKnowledgePtr = (uint8_t *)ACurrentPtr;
						ACurrentPtr += 64;
						for( int j = 0; j < 128; ++j )
							*AKnowledgePtr++ = *CurieNeuronRegisters::NM_COMP;

						*ACurrentPtr++ = *CurieNeuronRegisters::NM_AIF;
						*ACurrentPtr++ = *CurieNeuronRegisters::NM_MINIF;
						*ACurrentPtr++ = *CurieNeuronRegisters::NM_CAT; // Move to the next neuron
					}
				}

//				*CurieNeuronRegisters::NM_NSR = ATempNSR;
				*CurieNeuronRegisters::NM_FORGET = 0;
//				*CurieNeuronRegisters::NM_NSR = 0x0010;
				*CurieNeuronRegisters::NM_RSTCHAIN = 0;

				ACurrentPtr = ANeuronData;
				for( int i=0; i < ANCount; ++i )
				{
					*CurieNeuronRegisters::NM_NCR = *ACurrentPtr++;
					uint8_t *AKnowledgePtr = (uint8_t *)ACurrentPtr;
					ACurrentPtr += 64;
					for( int j = 0; j < 128; ++j )
						*CurieNeuronRegisters::NM_COMP = *AKnowledgePtr++;

					*CurieNeuronRegisters::NM_AIF = *ACurrentPtr++;
					*CurieNeuronRegisters::NM_MINIF = *ACurrentPtr++;
					*CurieNeuronRegisters::NM_CAT = *ACurrentPtr++; // Move to the next neuron
				}

				delete [] ANeuronData;
			}

			*CurieNeuronRegisters::NM_NSR = ATempNSR; // set the NN back to its calling status
		}

	public:
		inline void FeaturesInputPin_o_Receive( void *_Data )
		{
			FFeaturesValue = *(TArray<float>*)_Data;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER
	> class Arduino101NeuronForgetOperation
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.Forget();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CategoryOutputPin,
		typename T_CompletedOutputPin,
		typename T_FeaturesOutputPin,
		typename T_InfluenceFieldOutputPin,
		typename T_MinInfluenceFieldOutputPin,
		typename T_NeuronCompletedOutputPin
	> class Arduino101NeuronReadKnowledgeOperation :
		public T_CategoryOutputPin,
		public T_CompletedOutputPin,
		public T_FeaturesOutputPin,
		public T_InfluenceFieldOutputPin,
		public T_MinInfluenceFieldOutputPin,
		public T_NeuronCompletedOutputPin
	{
	public:
		_V_PIN_( FeaturesOutputPin )
		_V_PIN_( CategoryOutputPin )
		_V_PIN_( InfluenceFieldOutputPin )
		_V_PIN_( MinInfluenceFieldOutputPin )
		_V_PIN_( NeuronCompletedOutputPin )
		_V_PIN_( CompletedOutputPin )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			int ANCount = *CurieNeuronRegisters::NM_NCOUNT;
			int ATempNSR = *CurieNeuronRegisters::NM_NSR;

			*CurieNeuronRegisters::NM_NSR = 0x0010;
			*CurieNeuronRegisters::NM_RSTCHAIN = 0;

			float *AFeaturesData = new float[ 128 ];

			for( int i=0; i < ANCount; i++ )
			{
				int ATemp = *CurieNeuronRegisters::NM_NCR;
				if( ATemp && 0x7F == C_OWNER.GetContext() )
					ATemp = *CurieNeuronRegisters::NM_CAT; // Move to the next neuron

				else
				{
					float *ADataPtr = AFeaturesData;
					for( int j = 0; j < 128; ++j )
						*ADataPtr++ = float( *CurieNeuronRegisters::NM_COMP ) / 255;

					T_FeaturesOutputPin::SetPinValue( TArray<float>( 128, AFeaturesData ));
					T_InfluenceFieldOutputPin::SetPinValue( *CurieNeuronRegisters::NM_AIF );
					T_MinInfluenceFieldOutputPin::SetPinValue( *CurieNeuronRegisters::NM_MINIF );
					T_CategoryOutputPin::SetPinValue( *CurieNeuronRegisters::NM_CAT );
					T_NeuronCompletedOutputPin::ClockPin();
				}
			}

			delete [] AFeaturesData;
			*CurieNeuronRegisters::NM_NSR = ATempNSR; // set the NN back to its calling status

			T_CompletedOutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Category,
		typename T_Features,
        typename T_Features_ApplyValues,
        typename T_Features_GetValue,
		typename T_InfluenceField,
		typename T_MinInfluenceField
//        typename T_Values
	> class Arduino101NeuronAddNeuronOperation :
		public T_Category,
		public T_Features,
		public T_InfluenceField,
		public T_MinInfluenceField
	{
	public:
		_V_PROP_( Category )
        _V_PROP_( Features )
		_V_PROP_( MinInfluenceField )
		_V_PROP_( InfluenceField )

//	public:
//		uint32_t _MaxIndex;
//		float *_Values;

//	public:
//		float &operator[]( uint32_t AIndex ) { return T_Features::GetValue( AIndex ); }

	protected:
		void LoadFeatures()
		{
			for( uint32_t i = 0; i <= MitovMin<uint32_t>( T_Features::GetCount() - 1, 127 ); ++i )
            {
                float AValue = T_Features::GetValue( i );
                T_Features_GetValue::Call( i, AValue );
				*CurieNeuronRegisters::NM_COMP = AValue * 255 + 0.5;
            }
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			int ATempNSR = *CurieNeuronRegisters::NM_NSR;
			*CurieNeuronRegisters::NM_NSR = 0x0010;

			*CurieNeuronRegisters::NM_NCR = C_OWNER.GetContext();
			LoadFeatures();

			*CurieNeuronRegisters::NM_AIF = InfluenceField();
			*CurieNeuronRegisters::NM_MINIF = MinInfluenceField();
			*CurieNeuronRegisters::NM_CAT = Category(); // Move to the next neuron

			*CurieNeuronRegisters::NM_NSR = ATempNSR; // set the NN back to its calling status
		}

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

//	public:
//		using inherited::inherited;

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Category,
		typename T_Features,
		typename T_InfluenceField,
		typename T_MinInfluenceField
	> class Arduino101NeuronAddNeuronOperationConst :
		public T_Category,
		public T_Features,
		public T_InfluenceField,
		public T_MinInfluenceField
	{
	public:
		_V_PROP_( Category )
        _V_PROP_( Features )
		_V_PROP_( MinInfluenceField )
		_V_PROP_( InfluenceField )

//	public:
//		uint32_t _MaxIndex;
//		const / *PROGMEM* / float *_Values;

	protected:
		void LoadFeatures()
		{
			for( uint32_t i = 0; i <= MitovMin<uint32_t>( _MaxIndex, 127 ); ++i )
				*CurieNeuronRegisters::NM_COMP = pgm_read_float( _Values + i ) * 255 + 0.5;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			int ATempNSR = *CurieNeuronRegisters::NM_NSR;
			*CurieNeuronRegisters::NM_NSR = 0x0010;

			*CurieNeuronRegisters::NM_NCR = C_OWNER.GetContext();
			LoadFeatures();

			*CurieNeuronRegisters::NM_AIF = InfluenceField();
			*CurieNeuronRegisters::NM_MINIF = MinInfluenceField();
			*CurieNeuronRegisters::NM_CAT = Category(); // Move to the next neuron

			*CurieNeuronRegisters::NM_NSR = ATempNSR; // set the NN back to its calling status
		}

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Neurons_ProcessNeurons
	> class Arduino101NeuronLoadKnowledgeOperation
	{
	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			int ATempNSR = *CurieNeuronRegisters::NM_NSR;
			*CurieNeuronRegisters::NM_NSR = 0x0010;

			*CurieNeuronRegisters::NM_NCR = C_OWNER.GetContext();

			T_Neurons_ProcessNeurons::Call();
/*
			for( int i = 0; i < FNeurons.size(); ++i )
			{
				FNeurons[ i ]->LoadFeatures();
				*CurieNeuronRegisters::NM_AIF = FNeurons[ i ]->InfluenceField;
				*CurieNeuronRegisters::NM_MINIF = FNeurons[ i ]->MinInfluenceField;
				*CurieNeuronRegisters::NM_CAT = FNeurons[ i ]->Category; // Move to the next neuron
			}
*/

			*CurieNeuronRegisters::NM_NSR = ATempNSR; // set the NN back to its calling status
		}

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Category,
		typename T_Features,
        typename T_Features_ApplyValues,
        typename T_Features_GetValue,
		typename T_InfluenceField,
		typename T_MinInfluenceField
	> class Arduino101NeuronItem :
		public T_Category,
		public T_Features,
		public T_InfluenceField,
		public T_MinInfluenceField
	{
	public:
		_V_PROP_( Category )
        _V_PROP_( Features )
		_V_PROP_( MinInfluenceField )
		_V_PROP_( InfluenceField )

//	public:
//		uint32_t _MaxIndex;
//		float *_Values;

	public:
		void SetIndexedValue( uint32_t AIndex )
        {
        }

	public:
//		void LoadFeatures()
		void ProcessNeurons()
		{
			for( uint32_t i = 0; i <= MitovMin<uint32_t>( T_Features::GetCount() - 1, 127 ); ++ i )
            {
                float AValue = T_Features::GetValue( i );
                T_Features_GetValue::Call( i, AValue );
				*CurieNeuronRegisters::NM_COMP = AValue * 255 + 0.5;
            }
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Category,
		typename T_InfluenceField,
		typename T_MinInfluenceField
	> class Arduino101NeuronItemConst :
		public T_Category,
		public T_InfluenceField,
		public T_MinInfluenceField
	{
	public:
		_V_PROP_( Category )
		_V_PROP_( MinInfluenceField )
		_V_PROP_( InfluenceField )

//	public:
//		uint32_t _MaxIndex;
//		const / *PROGMEM* / float *_Values;

	public:
//		void LoadFeatures()
		void ProcessNeurons()
		{
			for( uint32_t i = 0; i <= MitovMin<uint32_t>( _MaxIndex, 127 ); ++i )
				*CurieNeuronRegisters::NM_COMP = pgm_read_float( _Values + i ) * 255 + 0.5;
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Category,
		typename T_MaxInfluenceField,
		typename T_MinInfluenceField
	> class Arduino101CurieTrainNeurons :
		public T_Category,
		public T_MaxInfluenceField,
		public T_MinInfluenceField
	{
	public:
		_V_PROP_( Category )
		_V_PROP_( MinInfluenceField )
		_V_PROP_( MaxInfluenceField )

//	protected:
//		int32_t	FValue = 0;

/*
	public:
		void CategoryInputPin_o_Receive( void *_Data )
		{
			FValue = 
		}
*/
	public:
		void TrainInputPin_o_Receive( void *_Data )
		{
/*
			int ACount = 0;
			for( int i = 0; i < FOwner.FFeatures.size(); ++i )
				ACount += FOwner.FFeatures[ i ]->GetCount();

			uint8_t *ABuffer = new uint8_t[ ACount ];
			uint8_t *APtr = ABuffer;
			for( int i = 0; i < FOwner.FFeatures.size(); ++i )
				FOwner.FFeatures[ i ]->GetFeatures( APtr );

			Arduino101CurieNeurons::FNeurons.Learn( ABuffer, ACount, constrain( CategoryInputPin.Value, -32768, 32767 ));

			delete []ABuffer;
*/
			if( C_OWNER.IsKNN() )
				*CurieNeuronRegisters::NM_NSR = *CurieNeuronRegisters::NM_NSR | 0x20;

			else
				*CurieNeuronRegisters::NM_NSR = *CurieNeuronRegisters::NM_NSR & ~0x20;

//			*CurieNeuronRegisters::NM_GCR = C_OWNER.GetContext();
			if( C_OWNER.UseInfinityDistance() )
				*CurieNeuronRegisters::NM_GCR = ( C_OWNER.GetContext() & 0x7F ) | 0x80;

			else
				*CurieNeuronRegisters::NM_GCR = C_OWNER.GetContext() & 0x7F;

			*CurieNeuronRegisters::NM_MINIF = MinInfluenceField();
			*CurieNeuronRegisters::NM_MAXIF = MaxInfluenceField();

			C_OWNER.FillFeatures();

//			Serial.println( "TRAIN" );
			*CurieNeuronRegisters::NM_CAT = Category();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CategoryOutputPin,
		typename T_CountNeuronsOutputPin,
		typename T_DegeneratedOutputPin,
		typename T_DisagreenmentOutputPin,
		typename T_DistanceOutputPin,
		typename T_NeuronIndexOutputPin,
		typename T_RecognizedOutputPin
	> class Arduino101CurieRecognizeNeurons :
		public T_CategoryOutputPin,
		public T_CountNeuronsOutputPin,
		public T_DegeneratedOutputPin,
		public T_DisagreenmentOutputPin,
		public T_DistanceOutputPin,
		public T_NeuronIndexOutputPin,
		public T_RecognizedOutputPin
	{
	public:
		_V_PIN_( CategoryOutputPin )
		_V_PIN_( DistanceOutputPin )
		_V_PIN_( NeuronIndexOutputPin )
		_V_PIN_( DegeneratedOutputPin )
		_V_PIN_( RecognizedOutputPin )
		_V_PIN_( DisagreenmentOutputPin )
		_V_PIN_( CountNeuronsOutputPin )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
/*
			int ACount = 0;
			for( int i = 0; i < C_OWNER.FFeatures.size(); ++i )
				ACount += C_OWNER.FFeatures[ i ]->GetCount();

			uint8_t *ABuffer = new uint8_t[ ACount ];
			uint8_t *APtr = ABuffer;
			for( int i = 0; i < C_OWNER.FFeatures.size(); ++i )
				C_OWNER.FFeatures[ i ]->GetFeatures( APtr );
*/
			*CurieNeuronRegisters::NM_GCR = C_OWNER.GetContext();

			C_OWNER.FillFeatures();

			int AResult = *CurieNeuronRegisters::NM_NSR;

//			Arduino101CurieNeurons::FNeurons.Learn( ABuffer, ACount, constrain( CategoryInputPin.Value, -32768, 32767 ));
/*
			int Adistance;
			int ACategory;
			int ANid;
			int AResult = Arduino101CurieNeurons::FNeurons.Classify( ABuffer, ACount, &Adistance, &ACategory, &ANid );
*/
			switch( AResult )
			{
				case 4:
//					CategoryOutputPin.SendValue<int32_t>( ACategory );
					T_DisagreenmentOutputPin::SetPinValueHigh();
					T_RecognizedOutputPin::SetPinValueHigh();
					break;

				case 8:
//					CategoryOutputPin.SendValue<int32_t>( ACategory );
					T_DisagreenmentOutputPin::SetPinValueLow();
					T_RecognizedOutputPin::SetPinValueHigh();
					break;

				default:
					T_DisagreenmentOutputPin::SetPinValueLow();
					T_RecognizedOutputPin::SetPinValueLow();
			}

			uint32_t ANeuronCount = 0;

			for(;;)
			{
				uint32_t Adistance = *CurieNeuronRegisters::NM_DIST;
				if( Adistance == 0xFFFF )
					break;

				int ACategory = *CurieNeuronRegisters::NM_CAT;
				bool ADegenerated = ( ( ACategory & 0x8000 ) != 0 );
				ACategory &= 0x7FFF;

				uint32_t ANeuronId = *CurieNeuronRegisters::NM_NID;

				++ ANeuronCount;
				T_DegeneratedOutputPin::SetPinValue( ADegenerated );
				T_NeuronIndexOutputPin::SetPinValue( ANeuronId );
				T_DistanceOutputPin::SetPinValue( Adistance );
				T_CategoryOutputPin::SetPinValue( ACategory );
			}

			T_CountNeuronsOutputPin::SetPinValue( ANeuronCount );
//			delete []ABuffer;
		}

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif