// yabridge: a Wine VST bridge
// Copyright (C) 2020-2021 Robbert van der Helm
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "../vst3.h"
#include "plug-view-proxy.h"

class Vst3PluginProxyImpl : public Vst3PluginProxy {
   public:
    Vst3PluginProxyImpl(Vst3PluginBridge& bridge,
                        Vst3PluginProxy::ConstructArgs&& args);

    /**
     * When the reference count reaches zero and this destructor is called,
     * we'll send a request to the Wine plugin host to destroy the corresponding
     * object.
     */
    ~Vst3PluginProxyImpl();

    /**
     * We'll override the query interface to log queries for interfaces we do
     * not (yet) support.
     */
    tresult PLUGIN_API queryInterface(const Steinberg::TUID _iid,
                                      void** obj) override;

    // From `IAudioProcessor`
    tresult PLUGIN_API
    setBusArrangements(Steinberg::Vst::SpeakerArrangement* inputs,
                       int32 numIns,
                       Steinberg::Vst::SpeakerArrangement* outputs,
                       int32 numOuts) override;
    tresult PLUGIN_API
    getBusArrangement(Steinberg::Vst::BusDirection dir,
                      int32 index,
                      Steinberg::Vst::SpeakerArrangement& arr) override;
    tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) override;
    uint32 PLUGIN_API getLatencySamples() override;
    tresult PLUGIN_API
    setupProcessing(Steinberg::Vst::ProcessSetup& setup) override;
    tresult PLUGIN_API setProcessing(TBool state) override;
    tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) override;
    uint32 PLUGIN_API getTailSamples() override;

    // From `IComponent`
    tresult PLUGIN_API getControllerClassId(Steinberg::TUID classId) override;
    tresult PLUGIN_API setIoMode(Steinberg::Vst::IoMode mode) override;
    int32 PLUGIN_API getBusCount(Steinberg::Vst::MediaType type,
                                 Steinberg::Vst::BusDirection dir) override;
    tresult PLUGIN_API
    getBusInfo(Steinberg::Vst::MediaType type,
               Steinberg::Vst::BusDirection dir,
               int32 index,
               Steinberg::Vst::BusInfo& bus /*out*/) override;
    tresult PLUGIN_API
    getRoutingInfo(Steinberg::Vst::RoutingInfo& inInfo,
                   Steinberg::Vst::RoutingInfo& outInfo /*out*/) override;
    tresult PLUGIN_API activateBus(Steinberg::Vst::MediaType type,
                                   Steinberg::Vst::BusDirection dir,
                                   int32 index,
                                   TBool state) override;
    tresult PLUGIN_API setActive(TBool state) override;
    tresult PLUGIN_API setState(Steinberg::IBStream* state) override;
    tresult PLUGIN_API getState(Steinberg::IBStream* state) override;

    // From `IConnectionPoint`
    tresult PLUGIN_API connect(IConnectionPoint* other) override;
    tresult PLUGIN_API disconnect(IConnectionPoint* other) override;
    tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) override;

    // From `IEditController`
    tresult PLUGIN_API setComponentState(Steinberg::IBStream* state) override;
    // `IEditController` also contains `getState()` and `setState()`  functions.
    // These are identical to those defiend in `IComponent` and they're thus
    // handled in in the same function.
    int32 PLUGIN_API getParameterCount() override;
    tresult PLUGIN_API
    getParameterInfo(int32 paramIndex,
                     Steinberg::Vst::ParameterInfo& info /*out*/) override;
    tresult PLUGIN_API
    getParamStringByValue(Steinberg::Vst::ParamID id,
                          Steinberg::Vst::ParamValue valueNormalized /*in*/,
                          Steinberg::Vst::String128 string /*out*/) override;
    tresult PLUGIN_API getParamValueByString(
        Steinberg::Vst::ParamID id,
        Steinberg::Vst::TChar* string /*in*/,
        Steinberg::Vst::ParamValue& valueNormalized /*out*/) override;
    Steinberg::Vst::ParamValue PLUGIN_API
    normalizedParamToPlain(Steinberg::Vst::ParamID id,
                           Steinberg::Vst::ParamValue valueNormalized) override;
    Steinberg::Vst::ParamValue PLUGIN_API
    plainParamToNormalized(Steinberg::Vst::ParamID id,
                           Steinberg::Vst::ParamValue plainValue) override;
    Steinberg::Vst::ParamValue PLUGIN_API
    getParamNormalized(Steinberg::Vst::ParamID id) override;
    tresult PLUGIN_API
    setParamNormalized(Steinberg::Vst::ParamID id,
                       Steinberg::Vst::ParamValue value) override;
    tresult PLUGIN_API
    setComponentHandler(Steinberg::Vst::IComponentHandler* handler) override;
    Steinberg::IPlugView* PLUGIN_API
    createView(Steinberg::FIDString name) override;

    // From `IEditController2`
    tresult PLUGIN_API setKnobMode(Steinberg::Vst::KnobMode mode) override;
    tresult PLUGIN_API openHelp(TBool onlyCheck) override;
    tresult PLUGIN_API openAboutBox(TBool onlyCheck) override;

    // From `IPluginBase`
    tresult PLUGIN_API initialize(FUnknown* context) override;
    tresult PLUGIN_API terminate() override;

    // From `IProgramListData`
    tresult PLUGIN_API
    programDataSupported(Steinberg::Vst::ProgramListID listId) override;
    tresult PLUGIN_API getProgramData(Steinberg::Vst::ProgramListID listId,
                                      int32 programIndex,
                                      Steinberg::IBStream* data) override;
    tresult PLUGIN_API setProgramData(Steinberg::Vst::ProgramListID listId,
                                      int32 programIndex,
                                      Steinberg::IBStream* data) override;

    // From `IUnitData`
    tresult PLUGIN_API
    unitDataSupported(Steinberg::Vst::UnitID unitId) override;
    tresult PLUGIN_API getUnitData(Steinberg::Vst::UnitID unitId,
                                   Steinberg::IBStream* data) override;
    tresult PLUGIN_API setUnitData(Steinberg::Vst::UnitID unitId,
                                   Steinberg::IBStream* data) override;

    // From `IUnitInfo`
    int32 PLUGIN_API getUnitCount() override;
    tresult PLUGIN_API
    getUnitInfo(int32 unitIndex,
                Steinberg::Vst::UnitInfo& info /*out*/) override;
    int32 PLUGIN_API getProgramListCount() override;
    tresult PLUGIN_API
    getProgramListInfo(int32 listIndex,
                       Steinberg::Vst::ProgramListInfo& info /*out*/) override;
    tresult PLUGIN_API
    getProgramName(Steinberg::Vst::ProgramListID listId,
                   int32 programIndex,
                   Steinberg::Vst::String128 name /*out*/) override;
    tresult PLUGIN_API
    getProgramInfo(Steinberg::Vst::ProgramListID listId,
                   int32 programIndex,
                   Steinberg::Vst::CString attributeId /*in*/,
                   Steinberg::Vst::String128 attributeValue /*out*/) override;
    tresult PLUGIN_API
    hasProgramPitchNames(Steinberg::Vst::ProgramListID listId,
                         int32 programIndex) override;
    tresult PLUGIN_API
    getProgramPitchName(Steinberg::Vst::ProgramListID listId,
                        int32 programIndex,
                        int16 midiPitch,
                        Steinberg::Vst::String128 name /*out*/) override;
    Steinberg::Vst::UnitID PLUGIN_API getSelectedUnit() override;
    tresult PLUGIN_API selectUnit(Steinberg::Vst::UnitID unitId) override;
    tresult PLUGIN_API
    getUnitByBus(Steinberg::Vst::MediaType type,
                 Steinberg::Vst::BusDirection dir,
                 int32 busIndex,
                 int32 channel,
                 Steinberg::Vst::UnitID& unitId /*out*/) override;
    tresult PLUGIN_API setUnitProgramData(int32 listOrUnitId,
                                          int32 programIndex,
                                          Steinberg::IBStream* data) override;

    /**
     * The component handler the host passed to us during
     * `IEditController::setComponentHandler()`. When the plugin makes a
     * callback on a component handler proxy object, we'll pass the call through
     * to this object.
     */
    Steinberg::IPtr<Steinberg::Vst::IComponentHandler> component_handler;

    /**
     * If the host doesn't connect two objects directly in
     * `IConnectionPoint::connect` but instead connects them through a proxy,
     * we'll store that proxy here. This way we can then route messages sent by
     * the plugin through this proxy. So far this is only needed for Ardour.
     */
    Steinberg::IPtr<Steinberg::Vst::IConnectionPoint> connection_point_proxy;

    /**
     * An unmanaged, raw pointer to the `IPlugView` instance returned in our
     * implementation of `IEditController::createView()`. We need this to handle
     * `IPlugFrame::resizeView()`, since that expects a pointer to the view that
     * gets resized.
     *
     * XXX: This approach of course won't work with multiple views, but the SDK
     *      currently only defines a single type of view so that shouldn't be an
     *      issue
     */
    Vst3PlugViewProxyImpl* last_created_plug_view = nullptr;

    // The following pointers are cast from `host_context` if
    // `IPluginBase::initialize()` has been called

    Steinberg::FUnknownPtr<Steinberg::Vst::IHostApplication> host_application;

    // The following pointers are cast from `component_handler` if
    // `IEditController::setComponentHandler()` has been called

    Steinberg::FUnknownPtr<Steinberg::Vst::IUnitHandler> unit_handler;

   private:
    Vst3PluginBridge& bridge;

    /**
     * An host context if we get passed one through `IPluginBase::initialize()`.
     * We'll read which interfaces it supports and we'll then create a proxy
     * object that supports those same interfaces. This should be the same for
     * all plugin instances so we should not have to store it here separately,
     * but for the sake of correctness we will.
     */
    Steinberg::IPtr<Steinberg::FUnknown> host_context;
};
